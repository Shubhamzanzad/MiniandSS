#include "login.h"

void view_all_courses(int desc){}
void enroll_new_course(int desc) {}
void drop_course(int desc) {}
void view_enrolled_course_details(int desc) {}
void change_password(int desc) {}
void stud_login_handler (int desc) {
    struct prof professor;
    struct stud student;
    if(login(false, true, desc, NULL, NULL)) {
        ssize_t write_size, read_size;
        char read_buff[1024], write_buff[1024];
        bzero(write_buff, 1024);
        bzero(read_buff, 1024);
        strcpy(write_buff, LOGIN_SUCCESS);
        while(1) {
            write_size = write(desc, STUD_MENU, sizeof(STUD_MENU));
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
                    view_all_courses(desc);
                    break;
                case 2:
                    enroll_new_course(desc);
                    break;
                case 3:
                    drop_course(desc);
                    break;
                case 4:
                    view_enrolled_course_details(desc);
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