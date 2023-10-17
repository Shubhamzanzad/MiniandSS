bool login(bool is_admin, bool is_prof, int desc, int id) {
    ssize_t read_size, write_size;
    char read_buff[1024], write_buff[1024], temp_buff[1024];
    struct stud student;
    struct prof professor;

    // int id;
    // bzero(read_buff, 1024);
    // bzero(write_buff, sizeof(write_buff));
    // write_size = write(desc, ASK_CREDENTIALS, sizeof(ASK_CREDENTIALS));
    // if(write_size == -1)
    //     perror("MESSAGE NOT DELIVERED");
    // bzero(write_buff, 1024);
    // bzero(temp_buff, 1024);
    // write_size = write(desc, ASK_LOGIN, sizeof(ASK_LOGIN));
    // if(write_size == -1) { 
    //     perror("COULDN'T ASK FOR LOGIN id");
    //     _exit(0);
    // } 
    // read_size = read(desc, read_buff, sizeof(read_buff));
    // if(read_size == -1) {
    //     perror("COULDN'T READ LOGIN ID");
    //     _exit(0);
    // }
    // int id = atoi(read_buff);
    bool user_found = false;
    if(is_admin) {
        if(strcmp(read_buff, ADMIN_LOGIN_ID))
            user_found = true;
        // bzero(write_buff, sizeof(write_buff));
        // write(desc, "user found", 11);
    }
    else if(is_prof) {
        bzero(temp_buff, sizeof(temp_buff));
        strcpy(temp_buff, read_buff);
        strtok(temp_buff, "-");
        id = atoi(strtok(NULL, "-"));

        int prof_file_fd = open("./prof_file", O_RDONLY);
        if (prof_file_fd == -1)
        {
            perror("Error opening professor file in read mode!");
            return false;
        }

        off_t offset = lseek(prof_file_fd, (id-1) * sizeof(struct prof), SEEK_SET);
        if (offset >= 0)
        {
            struct flock lock = {F_RDLCK, SEEK_SET, (id-1) * sizeof(struct prof), sizeof(struct prof), getpid()};

            int lockingStatus = fcntl(prof_file_fd, F_SETLKW, &lock);
            if (lockingStatus == -1)
            {
                perror("Error obtaining read lock on customer record!");
                return false;
            }

            read_size = read(prof_file_fd, &professor, sizeof(struct prof));
            if (read_size == -1)
            {
                perror("Error reading customer record from file!");
                return false;
            }
            int prof_id = atoi(read_buff);
            lock.l_type = F_UNLCK;
            fcntl(prof_file_fd, F_SETLK, &lock);

            if (professor.prof_id == prof_id)
                user_found = true;
            else    
                printf("Professor not found");

            close(prof_file_fd);
        }
    }    
    else {
        bzero(temp_buff, sizeof(temp_buff));
        strcpy(temp_buff, read_buff);
        strtok(temp_buff, "-");
        int stud_id = atoi(strtok(NULL, "-"));

        int stud_file_fd = open("./stud_file", O_RDONLY);
        if(stud_file_fd == -1)
        {
            perror("Error opening student file in read mode!");
            return false;
        }

        off_t offset = lseek(stud_file_fd, (id-1) * sizeof(struct stud), SEEK_SET);
        if (offset >= 0)
        {
            struct flock lock = {F_RDLCK, SEEK_SET, (id -1)* sizeof(struct stud), sizeof(struct stud), getpid()};

            int lockingStatus = fcntl(stud_file_fd, F_SETLKW, &lock);
            if (lockingStatus == -1)
            {
                perror("Error obtaining read lock on professor record!");
                return false;
            }

            read_size = read(stud_file_fd, &student, sizeof(struct stud));
            if (read_size == -1)
            {
                perror("Error reading customer record from file!");
                return false;
            }
        
            lock.l_type = F_UNLCK;
            fcntl(stud_file_fd, F_SETLK, &lock);

            stud_id = atoi(read_buff);
            if (student.stud_id == id)
                user_found = true;
            else 
                printf("Student not found");

            close(stud_file_fd);
       }
    }

    if (user_found){
        bzero(write_buff, sizeof(write_buff));
        write_size = write(desc, ASK_PASSWORD, sizeof(ASK_PASSWORD));
        if (write_size == -1)
        {
            perror("ERROR WHILE ASKING FOR PASSWORD");
            return false;
        }

        bzero(read_buff, sizeof(read_buff));
        read_size = read(desc, read_buff, sizeof(read_buff));
        if (read_size == -1)
        {
            perror("ERROR WHILE READING PASSWORD");
            return false;
        }
        if (is_admin) {
            if (strcmp(read_buff, ADMIN_PASSWORD) == 0)
                return true;
        }
        else if(is_prof)
        {
            if (strcmp(read_buff, professor.prof_password) == 0)
            {
                //*prof_id = professor;
                return true;
            }
        }
        else {
            if(student.flag == 1) {
                if (strcmp(read_buff, student.stud_password) == 0)
                {
                    //*stud_id = student;
                    return true;
                }
            }
            else {
                write(desc, "Student is deactivated.", sizeof("Student is deactivated."));
                return false;
            }
        }

        bzero(write_buff, sizeof(write_buff));
        write_size = write(desc, INVALID_PASSWORD, sizeof(INVALID_PASSWORD));
    }
    else
    {
        bzero(write_buff, sizeof(write_buff));
        write_size = write(desc, INVALID_LOGIN, sizeof(INVALID_LOGIN));
    }
    return false;
}      
