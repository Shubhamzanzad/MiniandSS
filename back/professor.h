#include "login.h"

void view_offering_courses(int desc){}
void add_new_course(int desc) {}
void remove_course_from_catalog(int desc) {}
void update_course_details(int desc) {}
void change_password(int desc) {}
void prof_login_handler (int desc) {
    struct prof professor;
    struct stud student;
    if(login(false, false, desc, NULL, NULL)) {
        ssize_t write_size, read_size;
        char read_buff[1024], write_buff[1024];
        bzero(write_buff, 1024);
        bzero(read_buff, 1024);
        strcpy(write_buff, LOGIN_SUCCESS);
        while(1) {
            write_size = write(desc, PROF_MENU, sizeof(PROF_MENU));
            if(write_size == -1) {
                perror("COULDN'T WRITE");
                _exit(0);
            } 
            bzero(write_buff, 1024);
            read_size = read(desc, read_buff, sizeof(read_buff));
            if(read_size == -1) {
                perror("COULDN'T READ");
                _exit(0);
            }
            int ch = atoi(read_buff);
            printf("%d", ch);
            switch(ch) {
                case 1:
                    view_offering_courses(desc);
                    break;
                case 2:
                    add_new_course(desc);
                    break;
                case 3:
                    remove_course_from_catalog(desc);
                    break;
                case 4:
                    update_course_details(desc);
                    break;
                case 5:
                    change_password(desc);
                    break;
                default:
                    write(desc, LOGOUT_MSG, sizeof(LOGOUT_MSG));
                    exit(EXIT_SUCCESS);
            }
        }
    }
}