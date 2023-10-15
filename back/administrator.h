#include<fcntl.h>
#include<stdlib.h>
#include<unistd.h>
#include<stdio.h>
#include<string.h>

#include "constants.h"
#include "msg.h"
//#include"structs.h"
#include"login.h"
// #include "student.h"
// #include "professor.h"
// #include "admin.h"
// #include "stud.h"
// #include "prof.h"

void add_new_student(int desc){
    ssize_t write_size, read_size;
    char write_buff[1024], read_buff[1024];

    bzero(write_buff, 1024);
    bzero(read_buff, 1024);

    struct stud new_student, last_student;
    
    int stud_file_fd = open("./stud_file", O_RDWR);
    if(stud_file_fd == -1 && errno == ENOENT) {
        new_student.stud_id = 1;
    }
    else if(stud_file_fd == -1) {
        perror("COULDN'T OPEN STUDENT FILE");
        _exit(0);
    }
    else {
        int offset = lseek(stud_file_fd, sizeof(struct stud), SEEK_END);
        if(offset == -1) {
            perror ("ERROR IN ACCESSING STUDENT RECORD");
            _exit(0);
        }

        struct flock lock = {F_RDLCK, SEEK_SET, offset, sizeof(struct stud), getpid()};
        int locking_stat = fcntl(stud_file_fd, F_SETLKW, &lock);
        if(locking_stat == -1) {
            perror("ERROR LOCKING THE FILE");
            _exit(0);
        }
        read_size = read(stud_file_fd, &last_student, sizeof(struct stud));
        if(read_size == -1) {
            perror("ERROR WHILE READING");
            _exit(0);
        } 

        lock.l_type = F_UNLCK;
        fcntl(stud_file_fd, F_SETLK, &lock);
        close(stud_file_fd);

        new_student.stud_id = last_student.stud_id + 1;
    }

    bzero(read_buff, 1024);
    write_size = write(desc, ASK_NAME, sizeof(ASK_NAME));
    if(write_size == -1) {
        perror("COULDN'T WRITE");
        _exit(0);
    }

    read_size = read(desc, read_buff, sizeof(read_buff));
    if(read_size == -1) {
        perror("COULDN'T READ");
        _exit(0);
    }

    strcpy(new_student.stud_name, read_buff);

    bzero(read_buff, 1024);

    write_size = write(desc, ASK_AGE, sizeof(ASK_AGE));
    if(write_size == -1) {
        perror("COULDN'T ASK AGE");
        _exit(0);
    }
    read_size = read(desc, read_buff, sizeof(read_buff));
    if(read_size == -1) {
        perror("COULDN'T READ THE AGE");
        _exit(0);
    }
    new_student.stud_age = atoi(read_buff);

    bzero(read_buff, 1024);

    write_size = write(desc, ASK_ADDRESS, sizeof(ASK_ADDRESS));
    // if(write_size = -1) {
    //     perror("COULDN'T ASK ADDRESS OF STUDENT");
    //     _exit(0);
    // }
    read_size = read(desc, read_buff, sizeof(read_buff));
    if(read_size == -1) {
        perror("COULDN'T READ ADDRESS OF STUDENT");
        _exit(0);
    }

    bzero(read_buff, 1024);
    strcpy(new_student.stud_address, read_buff);

    write_size = write(desc, ASK_EMAIL, sizeof(ASK_EMAIL));
    if(write_size == -1) {
        perror("COULDN'T ASK EMAIL");
        _exit(0);
    }

    read_size = read(desc, read_buff, sizeof(read_buff));
    if(read_size == -1) {
        perror("COULDN'T READ THE EMAIL");
        _exit(0);
    }

    strcpy(new_student.stud_email, read_buff);

    new_student.flag = 1;
    strcpy(new_student.stud_password, DEFAULT_PASSWORD);

    stud_file_fd = open("./stud_file", O_RDWR );
    if(stud_file_fd == -1) {
        perror("ERROR OPENING STUDENT FILE");
        _exit(0);
    }
    
    write_size = write(stud_file_fd, &new_student, sizeof(new_student));
    if(write_size == -1) {
        perror("COULDN'T WRITE STUDENT DETAILS TO THE FILE");
        _exit(0);
    }
    write(desc, "Student Added successfully to file.^", sizeof("Student Added successfully to file."));

    close(stud_file_fd);
    bzero(write_buff, sizeof(write_buff));
    sprintf(write_buff, "Name of student: %s\n ID: %d\nPassword: %s", new_student.stud_name, new_student.stud_id, new_student.stud_password);
    strcat(write_buff, "^");
    write_size = write(desc, write_buff, sizeof(write_buff));
    if(write_size = -1) {
        perror("ERROR IN SHOWING GENERATED ID");
        _exit(0);
    }
    bzero(read_buff, sizeof(read_buff));
    read(desc, read_buff, sizeof(read_buff));
    
}

void modify_student_details(int desc) { 
    ssize_t write_size, read_size;
    char read_buff[1024], write_buff[1024];
    bzero(write_buff, sizeof(write_buff));
    bzero(read_buff, sizeof(read_buff));

    struct stud student;
    
    int stud_file_fd = open("./stud_file", O_WRONLY);
    if(stud_file_fd == -1) {
        perror("ERROR IN OPENING STUDENT FILE IN WRITE MODE");
        _exit(0);
    }
    struct flock lock = {F_WRLCK, SEEK_SET, 0, sizeof(struct stud), getpid()};
    write_size = write(desc, ASK_ID, sizeof(ASK_ID));
    if(write_size == -1) {
        perror("COULD NOT ASK FOR STUDENT TO MODIFY DETAILS");
        _exit(0);
    }
    read_size = read(desc, read_buff, sizeof(read_buff));
    if(read_size == -1) {
        perror("ERROR IN READING STUDENT ID FROM CLIENT");
        _exit(0);
    }

    int id = atoi(read_buff);
    off_t offset = lseek(stud_file_fd, (id-1) * sizeof(struct stud), SEEK_SET);
    if(errno == EINVAL) {
        strcpy(write_buff, "STUDENT RECORD DOES NOT EXIST");
        strcat(write_buff, "^");
        write_size = write(desc, write_buff, sizeof(write_buff));
        if(write_size == -1) {
            perror("ERROR IN SENDING ERROR MESSAGE");
        }
        read_size = read(desc, read_buff, sizeof(read_buff));
        return;
    }
    else if(offset == -1) {
        perror("ERROR WHILE ACCESSING STUDENT RECORD");
        return;
    }
    lock.l_start = offset;
    int locking_status = fcntl(stud_file_fd, F_SETLKW, &lock);
    if(locking_status == -1) {
        perror("ERROR IN LOCKING");
        return;
    }
    // if(student.flag == 1) {
        write_size = write(desc, ASK_NAME, sizeof(ASK_NAME));
        if(write_size == -1) {
            perror("ERROR ASKING FOR NEW NAME");
            _exit(0);
        }
        bzero(read_buff, sizeof(read_buff));
        read_size = read(desc, read_buff, sizeof(read_buff));
        if(read_size == -1) {
            perror ("ERROR IN READING NEW NAME");
            _exit(0);
        }
        strcpy(student.stud_name, read_buff);

        write_size = write(desc, ASK_AGE, sizeof(ASK_AGE));
        if(write_size == -1) {
            perror("ERROR ASKING FOR NEW AGE");
            _exit(0);
        }
        bzero(read_buff, sizeof(read_buff));
        read_size = read(desc, read_buff, sizeof(read_buff));
        if(read_size == -1) {
            perror ("ERROR IN READING NEW AGE");
            _exit(0);
        }
        int age = atoi(read_buff);
        student.stud_age = age;
        
        write_size = write(desc, ASK_EMAIL, sizeof(ASK_EMAIL));
        if(write_size == -1) {
            perror("ERROR ASKING FOR NEW EMAIL");
            _exit(0);
        }
        bzero(read_buff, sizeof(read_buff));
        read_size = read(desc, read_buff, sizeof(read_buff));
        if(read_size == -1) {
            perror ("ERROR IN READING NEW EMAIL");
            _exit(0);
        }
        strcpy(student.stud_email, read_buff);

        write_size = write(desc, ASK_ADDRESS, sizeof(ASK_ADDRESS));
        if(write_size == -1) {
            perror("ERROR ASKING FOR NEW ADDRESS");
            _exit(0);
        }
        bzero(read_buff, sizeof(read_buff));
        read_size = read(desc, read_buff, sizeof(read_buff));
        if(read_size == -1) {
            perror ("ERROR IN READING NEW ADDRESS");
            _exit(0);
        }
        strcpy(student.stud_address, read_buff);
        
        write_size = write(stud_file_fd, &student, sizeof(struct stud));
        if(write_size == -1) {
            perror("ERROR IN WRITING STUDENT DATA IN FILE");
            _exit(0);
        }
        write(desc, "Changes are written to the file.^", sizeof("Changes are written to the file.^"));
        
    // }
    // else {
    //     write(desc, "Student is been deactivated.^", sizeof("Student is been deactivated.^"));
    // }
    read(desc, read_buff, sizeof(read_buff));
}

void view_student_details(int desc) {
    ssize_t read_size, write_size;
    char read_buff[1024], write_buff[1024], temp_buff[1024];
    struct stud student;
    int stud_file_fd = open("./stud_file", O_RDONLY);
    int id;

    struct flock lock = {F_RDLCK, SEEK_SET, (id-1)*sizeof(struct stud), sizeof(struct stud), getpid()};

    bzero(write_buff, sizeof(write_buff));
    bzero(read_buff, sizeof(read_buff));
    write_size = write(desc, ASK_ID, sizeof(ASK_ID));
    if(write_size == -1) {
        perror("ERROR ASKING IN STUDENT ID");
        _exit(0);
    }
    read_size = read(desc, read_buff, sizeof(read_buff));
    if(read_size == -1) {
        perror("ERROR IN READING STUDENT ID");
        _exit(0);
    }
    id = atoi(read_buff);

    int offset = lseek(stud_file_fd, (id-1) * sizeof(struct stud), SEEK_SET);
    if(errno == EINVAL) {
        strcpy(write_buff, "STUDENT RECORD DOES NOT EXIST");
        strcat(write_buff, "^");
        write_size = write(desc, write_buff, sizeof(write_buff));
        if(write_size == -1) {
            perror("ERROR IN SENDING ERROR MESSAGE");
        }
        read_size = read(stud_file_fd, read_buff, sizeof(read_buff));
        return;
    }
    else if(offset == -1) {
        perror("ERROR WHILE ACCESSING STUDENT RECORD");
        return;write(desc,"Student is deactivated", sizeof("Student is deactivated"));
    }
    lock.l_start = offset;
    int locking_status = fcntl(stud_file_fd, F_SETLKW, &lock);
    if(locking_status == -1) {
        perror("ERROR IN LOCKING");
        return;
    }

    read_size = read(stud_file_fd, &student, sizeof(struct stud));
    if(read_size == -1) {
        perror("ERROR IN READING STUDENT RECORD FROM FILE");
        return;
    }

    lock.l_type = F_UNLCK;
    fcntl(stud_file_fd, F_SETLK, &lock);
    //if(student.flag == 1) {
        bzero(write_buff, sizeof(write_buff));
        sprintf(write_buff,"Student details are as follows\nRoll no.: %d\nName: %s\nAge: %d\nEmail: %s\nAddress: %s\n", student.stud_id, student.stud_name, student.stud_age, student.stud_email, student.stud_address);
        strcat(write_buff, "You will be directed to main page\n^");
        write_size = write(desc, write_buff, sizeof(write_buff));
        if(write_size == -1) {
            perror("ERROR IN GIVING OUT FINAL STUDENT DETAILS");
            return;
        }
    // }
    // else {
    //     write(desc, "Student is deactivated.^", sizeof("Student is deactivated.^"));
    // }
    close(stud_file_fd);
    read(desc, read_buff, sizeof(read_buff));
}

void deactivate_activate(int desc) {
    ssize_t read_size, write_size;
    char read_buff[1024], write_buff[1024];
    struct stud student;
    int stud_file_fd = open("./stud_file", O_RDWR);
    if(stud_file_fd == -1) {
        perror("ERROR OPENING STUDENT FILE");
        _exit(0);
    }
    struct flock lock = {F_WRLCK, SEEK_SET, 0, sizeof(struct stud), getpid()};
    bzero(read_buff, sizeof(read_buff));
    bzero(write_buff, sizeof(write_buff));

    write_size = write(desc, ASK_ID, sizeof(ASK_ID));
    if(write_size == -1) {
        perror("ERROR COULD NOT ASK FOR ROLL NO");
        _exit(0);
    }
    read_size = read(desc, read_buff, sizeof(read_buff));
    if(read_size == -1) {
        perror("ERROR READING ROLL NO FROM CLIENT");
        _exit(0);
    }
    int id = atoi(read_buff);
    int offset = lseek(stud_file_fd, (id-1) * sizeof(struct stud), SEEK_SET);
    
    if(offset == -1) {
        perror("FILE ERROR");
        _exit(0);
    }
    lock.l_start = offset;
    int locking_stat = fcntl(stud_file_fd, F_SETLKW, &lock);
    if(locking_stat == -1) {
        perror("ERROR IN LOCKING THE FILE");
        _exit(0);
    }

    if(student.flag == 0) {
        student.flag = 1;
        write(desc,"Student is been activated.^", sizeof("Student is been activated.^"));    
    }
    if(student.flag == 1) {
        student.flag = 0;
        write(desc,"Student is been deactivated.^", sizeof("Student is deactivated.^"));
    }
    write_size = write(stud_file_fd, &student, sizeof(student));
    if(write_size == -1) {
        perror("CANNOT CHANGE THE STUDENT TYPE IN FILE");
        _exit(0);
    }
    //write(desc, "Changes are written to the file.^", sizeof("Changes are written to the file.^"));

    lock.l_type = F_UNLCK;
    locking_stat = fcntl(stud_file_fd, F_SETLKW, &lock);
    if(locking_stat == -1) {
        perror("ERROR IN UNLOCKING THE FILE");
        return;
    }
    close(stud_file_fd);
    read(desc, read_buff, sizeof(read_buff));
}

void activate(int desc) {
    ssize_t read_size, write_size;
    char read_buff[1024], write_buff[1024];
    struct stud student;
    int stud_file_fd = open("./stud_file", O_RDWR);
    if(stud_file_fd == -1) {
        perror("ERROR OPENING STUDENT FILE");
        _exit(0);
    }
    struct flock lock = {F_WRLCK, SEEK_SET, 0, sizeof(struct stud), getpid()};
    bzero(read_buff, sizeof(read_buff));
    bzero(write_buff, sizeof(write_buff));

    write_size = write(desc, ASK_ID, sizeof(ASK_ID));
    if(write_size == -1) {
        perror("ERROR COULD NOT ASK FOR ROLL NO");
        _exit(0);
    }
    read_size = read(desc, read_buff, sizeof(read_buff));
    if(read_size == -1) {
        perror("ERROR READING ROLL NO FROM CLIENT");
        _exit(0);
    }
    int id = atoi(read_buff);
    int offset = lseek(stud_file_fd, id * sizeof(struct stud), SEEK_SET);
    
    if(offset == -1) {
        perror("FILE ERROR");
        _exit(0);
    }
    lock.l_start = offset;
    int locking_stat = fcntl(stud_file_fd, F_SETLKW, &lock);
    if(locking_stat == -1) {
        perror("ERROR IN LOCKING THE FILE");
        _exit(0);
    }

    student.flag = 1;
    write(desc,"Student is activated", sizeof("Student is activated"));

    lock.l_type = F_UNLCK;
    locking_stat = fcntl(stud_file_fd, F_SETLKW, &lock);
    if(locking_stat == -1) {
        perror("ERROR IN UNLOCKING THE FILE");
        return;
    }
    close(stud_file_fd);
}

void add_new_professor(int desc) {
    ssize_t write_size, read_size;
    char write_buff[1024], read_buff[1024];

    bzero(write_buff, 1024);
    bzero(read_buff, 1024);

    struct prof new_professor, last_professor;
    
    int prof_file_fd = open("./prof_file", O_RDWR);
    if(prof_file_fd == -1) {
        perror("CANNOT OPEN PROFESSOR FILE");
        _exit(0);
    }
    if(prof_file_fd == -1 && errno == ENOENT) {
        new_professor.prof_id = 1;
    }
    else if(prof_file_fd == -1) {
        perror("COULDN'T OPEN PROFESSOR FILE");
        _exit(0);
    }
    else {
        int offset = lseek(prof_file_fd, sizeof(struct prof), SEEK_END);
        if(offset == -1) {
            perror ("ERROR IN ACCESSING STUDENT RECORD");
            _exit(0);
        }

        struct flock lock = {F_RDLCK, SEEK_SET, offset, sizeof(struct prof), getpid()};
        int locking_stat = fcntl(prof_file_fd, F_SETLKW, &lock);
        if(locking_stat == -1) {
            perror("ERROR LOCKING THE FILE");
            _exit(0);
        }
        read_size = read(prof_file_fd, &last_professor, sizeof(struct prof));
        if(read_size == -1) {
            perror("ERROR WHILE READING");
            _exit(0);
        } 

        lock.l_type = F_UNLCK;
        fcntl(prof_file_fd, F_SETLK, &lock);
        close(prof_file_fd);

        new_professor.prof_id = new_professor.prof_id + 1;
    }

    bzero(write_buff, 1024);
    bzero(read_buff, 1024);
    write_size = write(desc, ASK_NAME, sizeof(ASK_NAME));
    if(write_size == -1) {
        perror("COULDN'T WRITE");
        _exit(0);
    }

    read_size = read(desc, read_buff, sizeof(read_buff));
    if(read_size == -1) {
        perror("COULDN'T READ");
        _exit(0);
    }

    strcpy(new_professor.prof_name, read_buff);

    bzero(write_buff, 1024);
    bzero(read_buff, 1024);

    write_size = write(desc, ASK_AGE, sizeof(ASK_AGE));
    if(write_size == -1) {
        perror("COULDN'T ASK AGE");
        _exit(0);
    }
    read_size = read(desc, read_buff, sizeof(read_buff));
    if(read_size == -1) {
        perror("COULDN'T READ THE AGE");
        _exit(0);
    }
    new_professor.prof_age = atoi(read_buff);

    bzero(write_buff, 1024);
    bzero(read_buff, 1024);

    write_size = write(desc, ASK_EMAIL, sizeof(ASK_EMAIL));
    if(write_size == -1) {
        perror("COULDN'T ASK EMAIL");
        _exit(0);
    }
    read_size = read(desc, read_buff, sizeof(read_buff));
    if(read_size == -1) {
        perror("COULDN'T READ THE EMAIL");
        _exit(0);
    }

    strcpy(new_professor.prof_email, read_buff);

    bzero(write_buff, 1024);
    bzero(read_buff, 1024);

    strcpy(new_professor.prof_password, DEFAULT_PASSWORD);

    prof_file_fd = open("./prof_file", O_WRONLY);
    if(prof_file_fd == -1) {
        perror("ERROR OPENING PROFESSOR FILE");
        _exit(0);
    }
    //bzero(write_buff, sizeof(write_buff));
    write_size = write(prof_file_fd, &new_professor, sizeof(new_professor));
    if(write_size == -1) {
        perror("COULDN'T WRITE PROFESSOR DETAILS TO THE FILE");
        _exit(0);
    }
    close(prof_file_fd);
    write(desc, "Changes written to professor file.^", sizeof("Changes written to professor file.^"));
    read(desc, read_buff, sizeof(read_buff));

}

void admin_login_handler(int desc) {
    struct prof professor;
    struct stud student;
    if(login(true, false, desc, NULL, NULL)) {
        ssize_t write_size, read_size;
        char read_buff[1024], write_buff[1024];
        bzero(write_buff, 1024);
        bzero(read_buff, 1024);
        strcpy(write_buff, LOGIN_SUCCESS);
        while(1) {
            write_size = write(desc, ADMIN_MENU, sizeof(ADMIN_MENU));
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
                    add_new_student(desc);
                    break;
                case 2:
                    view_student_details(desc);
                    break;
                case 3:
                    modify_student_details(desc);
                    break;
                case 4:
                    deactivate_activate(desc);
                    break;
                // case 5:
                //     activate(desc);
                //     break;
                case 5:
                    add_new_professor(desc);
                    break;
                default:
                    write(desc, LOGOUT_MSG, sizeof(LOGOUT_MSG));
                    exit(EXIT_SUCCESS);

            }
        }
    }
}

