#include "../master.h"
#include "../applications/snake/snake.h"

char term_buffer[100] = {0};
int run = 1;

void eval(char* equ) { // not implemented
    int num1;
    int num2;
    while(*equ) {
        if(isdigit(*equ)) {
            
        }
    }
}

//CLEAN THIS UP
char* help = "Help:\n\t.exit: end the program completely\n\techo: prints the input given\n\t.write: enters writemode, exits shell, press esc to exit back to shell\n\t.save: saves the state of the screen\n\t.load: loads the saved state of the screen\n\t.read_file: prints the given file\n\t.write_file: enters write mode to write a file, saved on exit.\n\t.ls: prints a list of all the files\n";
void exec() {
    char temp_buff[100]; // holds arguments
    static char buff_filename[20]; // holds the filename when new files are being created and loaded and keeps it static between execs
    char *cmd = lsplit(term_buffer, ' ', temp_buff);
    if(strcmp(cmd, ".exit") == 0) { //if the term buffer hold exit
        strcpy(term_buffer, ""); // resets the buffer
        run = 0; // stops the main loops
        return; // end exec
    } else if (strcmp(lsplit(term_buffer, ' ', temp_buff), "echo") == 0) { //puts the command into the temp buffer
                //checks if the command buffer is the command 'echo'
        char *arg = term_buffer + strlen("echo") + 1; // advanced to the char after the end of "echo"
        if (*arg == '\0'){ // empty argument
            error(2); // empty error
            strcpy(term_buffer, ""); // resets the buffer
            return;
        }
        term_putstr(arg); //prints the input, if echo was made proper
        term_putchar('\n'); //then goes to the new psh line
        return;
    } else if(strcmp(term_buffer, ".write") == 0){
        state.write = 1;
        return;
    } else if (strcmp(term_buffer, "sys.write_disable") == 0) {
        term_putchar('\n'); //so psh starts on next line
        return;
    } else if (strcmp(term_buffer, ".help") == 0) {
        term_putstr(help);
        return;
    } else if(strcmp(term_buffer, ".save") == 0) {
        save();
        term_putchar('\n');
    } else if(strcmp(term_buffer, ".load") == 0) {
        if(state.loaded == 0) {
            init_vga(); //clears vga
            memcopy((char*)VGA_MEMORY, load(), stored_vga_length*2);
            curser_loc(stored_vga_length*2);
        } else {
            error(9);
            return;
        }
        term_putchar('\n');
    } else if(strcmp(lsplit(term_buffer, ' ', temp_buff), ".read_file") == 0) {
        strcpy(buff_filename ,term_buffer + strlen(".read_file") + 1); // advanced to the char after the end of ".read", holds a pointer to that spot and acts as an array til char 20
        if (*buff_filename == '\0'){ // empty argument
            error(2); // empty error
            strcpy(term_buffer, ""); // resets the buffer
            return;
        }
        vga_char loaded_file[VGA_WIDTH * VGA_HEIGHT];
        read_file(buff_filename, loaded_file);
        put_vga_str(loaded_file);
        //memcopy((char*)VGA_MEMORY, loaded_file, stored_vga_length*2);
        term_putchar('\n');
        return;
    } else if(strcmp(lsplit(term_buffer, ' ', temp_buff), ".write_file") == 0) {
        char* namepart = term_buffer + strlen(".write_file") + 1;
        if (*namepart == '\0'){ // checks if empty argument
            error(2); // empty error
            strcpy(term_buffer, ""); // resets the buffer
            return;
        }
        strcpy(buff_filename, namepart);  // actually save it
        init_vga(); // clears vga
        curser_loc(0); // curser goes to 0.
        state.write = 1; // enters write mode state
        state.write_file = 1;
        return; // shell handles the rest 
    } else if(strcmp(term_buffer, "sys.finishfile") == 0) {
        write_file(buff_filename, load());
        state.write_file = 0;
        term_putchar('\n');
        return; 
    } else if(strcmp(".clear", cmd) == 0) {
        init_vga();
        curser_loc(0);
        return;
    } else if(strcmp(cmd, ".ls") == 0) {
        char *buff[INODE_COUNT]; // tells ls how many strings are in this array. 
        ls(buff); // loads all the strings into array
        int i = 0;
        while(buff[i] != NULL) { //checks if the string its at is null
            term_putstr(buff[i]); // put the string at buff 
            term_putchar('\n');
            i++; //incrememnt 1 string 
        }
    } else if(strcmp(cmd, ".snake") == 0) { 
        init_snake();
        snake();
        return;
    } else {
        error(1); // not found error
        strcpy(term_buffer, ""); // resets the buffer
        return;
    } // add free write mode maybe
}

/*
        uint16_t cha = key_reader(); // take it as a num for extd keys, then read it into the char
        if((cha & 0x100) == 0x100) { // if this is an extended key
            extd_keys(cha); // do the extended key
            continue;
        }
*/

void shell(){        
    term_putstr("psh>> "); //shell name, starts at every new command prompt
    int term_buffer_loc = 0; //buffer holds what is currently being typed
    while(run) {
        char ch = key_map(key_reader()); // if 0, continues due to next line
        if(ch != NULL) { 
            if(ch == '\n'){ // new line is enter
                term_buffer[term_buffer_loc] = '\0'; // ends buffer
                term_putchar('\n'); // next line so exec operates there
                disabled_writemode:
                if(state.write == 0) {
                    exec(); // executes whats there
                    term_buffer[0] = '\0'; // resets the buffer
                    term_buffer_loc = 0; // resets the position of the buffer
                    term_putstr("psh>> "); // new shell prompt
                }
                continue;
            }
            if((ch == 0x01) && !state.write) { // if write mode is off
                strcpy(term_buffer, ".exit");
                exec(); // executes exit command
                continue;
            } else if ((ch == 0x01) && state.write) {
                state.write = 0; // writemode off 
                if(state.write_file == 1) {
                    save();
                    strcpy(term_buffer, "sys.finishfile");
                } else {
                    strcpy(term_buffer, "sys.write_disable");
                }
                goto disabled_writemode; // goes up to spot where writemode is off
            }
            if(ch == '\b') { 
                term_buffer[term_buffer_loc] = '\0'; // positon is terminator
                term_buffer_loc--; // goes back 1
                term_putchar(ch & 0x7F); // put char deals with '\b'
                continue; // does not move up 1 so it doesnt go back to orig spot
            }
            term_putchar(ch & 0x7F); //0x7F are the key bits, so does not have release or press 
            term_buffer[term_buffer_loc] = ch; // puts the new character into the buffer
            term_buffer_loc++; //goes up 1
        } else {
            continue; // if the key is not found, ignore
        }
    }
}