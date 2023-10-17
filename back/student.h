struct enroll get_student_enrollment(int desc, int sid, int cid){
    ssize_t read_size, write_size;
    char read_buff[1024], write_buff[1024];
    int enroll_file_fd = open("./enroll",O_RDWR|O_APPEND);
    struct enroll e={0,0,0,0,0};
    if (enroll_file_fd == -1) {
        exit(1);
    }
    if(get_count(3)<cid){
        write(desc,"Wrong Course id.^",sizeof("Wrong Course id.^"));
        return e;
    }
    if(get_count(1)<sid){
        write(desc,"Wrong Student id.^",sizeof("Wrong Student id.^"));
        return e;
    }
    struct flock lock = {F_RDLCK, SEEK_SET, 0, 0, getpid()};
    // lock.l_type = F_RDLCK;
    // lock.l_whence = SEEK_SET;
    // lock.l_start = 0;
    // lock.l_len = 0;
    // lock.l_pid = getpid();
    int locking_stat = fcntl(enroll_file_fd, F_SETLKW, &lock);
    if(locking_stat==-1){
        perror("Error obtaining Read lock on Enrollments Record!");
        close(enroll_file_fd);
    }
    lseek(enroll_file_fd,0,SEEK_SET);
    // int bytesRead;

    while((read_size = read(enroll_file_fd ,&e ,sizeof(struct enroll))) == sizeof(struct enroll) || read_size == -1){
        if(e.stud_id == sid && e.course_id == cid){
            break;
        }
        bzero(read_buff, sizeof(read_buff));
    }
    lock.l_type = F_UNLCK;
    fcntl(enroll_file_fd,F_SETLK,&lock);
    close(enroll_file_fd);
    return e;
}

void view_all_courses(int desc){
    ssize_t read_size, write_size;
    char read_buff[1024], write_buff[1024];
    int course_file_fd = open("./courses", O_RDONLY);
    if(course_file_fd == -1) {
        perror("ERROR IN OPENING COURSE FILE");
        _exit(0);
    }
    struct flock lock = {F_RDLCK, SEEK_SET, 0, 0, getpid()};
    int locking_stat = fcntl(course_file_fd, F_SETLKW, &lock);
    if(locking_stat == -1) {
        perror("ERROR IN LOCKING FILE");
        _exit(0);
    }
    lseek(course_file_fd, 0, SEEK_SET);
    int cnt = get_count(3);
    if(cnt == 0) {
        write(desc, "No Courses found.^", sizeof("No Courses found.^"));
    }
    struct course cr;
    while((read_size = read(course_file_fd, &cr, sizeof(struct course))) == sizeof(struct course) || read_size == -1) {
        char transfer[100+sizeof(cr)];
        sprintf(transfer, "Course name: %s\nCredits: %d\n Total seats: %d.^", 
        cr.course_name, cr.credit, cr.seats);
        write_size = write(desc, transfer, sizeof(transfer));
        if(write_size == -1) {
            perror("ERROR IN TRANSFERING DATA");
            _exit(0);
        }
        read(desc, read_buff, sizeof(read_buff));//dummy read
        bzero(read_buff, sizeof(read_buff));
    }
    lock.l_type = F_UNLCK;
    fcntl(course_file_fd, F_SETLK, &lock);
    close(course_file_fd);
}

int *view_enrolled_course_details(int desc, int id, int *cnt, int flag) {
    ssize_t read_size, write_size;
    char read_buff[1024], write_buff[1024];
    int enroll_file_fd = open("./enroll",O_RDONLY);
    if (enroll_file_fd == -1) {
        exit(1);
    }
    int course_cnt = get_count(3);
    if(course_cnt == 0){
        write(desc,"No Courses Available.^",sizeof("No Courses Available.^"));
    }
    struct flock lock = {F_RDLCK, SEEK_SET ,0 ,0 , getpid()};
    // lock.l_type = F_RDLCK;
    // lock.l_whence = SEEK_SET;
    // lock.l_start = 0;
    // lock.l_len = 0;
    // lock.l_pid = getpid();
    int locking_stat = fcntl(enroll_file_fd, F_SETLKW, &lock);
    if(locking_stat == -1){
        perror("ERROR IN LOCKING THE FILE");
        close(enroll_file_fd);
    }
    lseek(enroll_file_fd,0,SEEK_SET);
    struct enroll e;
    // int bytesRead;
    int arraySize =1;
    int count = 0;
    int *cid = (int *)malloc(arraySize * sizeof(int));
    while((read_size=read(enroll_file_fd,&e,sizeof(struct enroll))) == sizeof(struct enroll) || read_size == -1){
        if(e.is_enrolled && e.stud_id == id){
            char send[100+sizeof(struct course)],skip[2];
            struct course c = get_courses(desc,e.course_id);
            if(flag){
                sprintf(send,"Course Name: %s\nCredits: %d\nTotal seats: %d\nAvailable Seats: %d\nId: %d^",c.course_name,c.credit,c.seats,c.available,c.course_id);
                write(desc,send,sizeof(send));
                bzero(send, sizeof(send));
                read_size = read(desc, read_buff, sizeof(read_buff));//dummy read
                bzero(send, sizeof(send));
            }
            arraySize += 1;
            cid = (int *)realloc(cid, arraySize * sizeof(int));
            if (cid == NULL) {
                perror("Memory reallocation failed");
            }
            cid[count++] = e.course_id;
        }
    }
    
    lock.l_type = F_UNLCK;
    fcntl(enroll_file_fd,F_SETLK,&lock);
    close(enroll_file_fd);
    *cnt = count;
    return cid;
}

void enroll_new_course(int desc, int id) {
    struct prof professor;
    struct stud student;
    ssize_t write_size, read_size;
    char read_buff[1024], write_buff[1024], temp_buff[1024];
    bzero(write_buff, 1024);
    bzero(read_buff, 1024);
    int enroll_file_fd = open("./enroll", O_RDWR | O_APPEND);
    if(enroll_file_fd == -1) {
        perror("ERROR OPENING ENROLL FILE");
        _exit(0);
    }
    int cnt = get_count(3);
    if(cnt == 0) {
        write(desc, "NO COURSES AVAILABLE", sizeof("NO COURSES AVAILABLE"));
    }
    int course_id;
    while(1) {
        write_size = write(desc, "Enter course_id to enroll", sizeof("Enter course_id to enroll"));
        if(write_size == -1) {
            perror("ERROR IN ASKING COURSE ID");
            _exit(0);
        }
        read_size = read(desc, read_buff, sizeof(read_buff));
        if(read_size == -1) {
            perror("ERROR IN READING");
            _exit(0);
        }
        course_id = atoi(read_buff);
        if(course_id <= 0 || get_count(3) < course_id) {
            write_size = write(desc, "Enter correct id", sizeof("Enter correct id"));
            _exit(0);
        }
    }
    //int cnt;
    int *enrolled = view_enrolled_course_details(desc, id, &cnt, 0);
    int flag = 0;
    for (int i = 0; i < cnt; i++) {
        if (enrolled[i] == id) {
            flag = 1;
            break; // Number is found, exit the loop
        }
    }
    if(flag == 1){
        write_size = write(desc,"Course already Enrolled...\n^",sizeof("Course already Enrolled...\n^"));
        return ;
    }
    else{
        struct flock lock = {F_WRLCK, SEEK_END, 0, sizeof(struct stud), getpid()};
        // lock.l_type = F_WRLCK;
        // lock.l_whence = SEEK_END;
        // lock.l_start = 0;
        // lock.l_len = sizeof(struct stud);
        // lock.l_pid = getpid();
        int locking_stat = fcntl(enroll_file_fd, F_SETLKW, &lock);
        if(locking_stat == -1){
            perror("Error obtaining write lock on Student Record!");
            close(enroll_file_fd);
        }
        struct course cr = get_courses(desc, course_id);
        if(cr.available > 0){
            int enrollId = set_count(4);
            set_availability(desc ,course_id ,-1 );
            struct enroll e = {id, cr.course_id, cr.prof_id, enrollId , 1};
            write_size = write(enroll_file_fd,&e,sizeof(struct enroll));
            if(write_size == -1) {
                perror("CANNOT WRITE IN FILE");
            }
            // char tempBuffer[100];
            sprintf(temp_buff,"\nSuccessfully Enrolled in the course - %s^",cr.course_name);
            write_size = write(desc,temp_buff,sizeof(temp_buff));
            if(write_size == -1) {
                perror("ERROR IN WRITING");
            } 
        }
        else {
            write(desc,"No seats availble.^", sizeof("No seats availble.^"));
            read(desc, read_buff, sizeof(read_buff));
        }
        lock.l_type = F_UNLCK;
        fcntl(enroll_file_fd, F_SETLK, &lock);
    }
    close(enroll_file_fd);
}

void drop_course(int desc, int id) {
    ssize_t read_size, write_size;
    char read_buff[1024], write_buff[1024];
    int fd = open("./enroll",O_RDWR);
    if (fd == -1) {
        exit(1);
    }
    int course_cnt = get_count(3);
    if(course_cnt == 0){
        write(desc,"No Courses Available.^",sizeof("No Courses Available.^"));
        return;
    }
    int cnt;
    int *enrolled = view_enrolled_course_details(desc, id,&cnt,0);
    if(cnt == 0){
        write(desc,"No Courses Enrolled.^",sizeof("No Courses Enrolled.^"));
        return;
    }
    int cid;
    char buff[10];
    struct flock lock;
    while(1){
        write_size = write(desc,"Enter the course ID number to Enroll: ",sizeof("Enter the course ID number to Enroll: "));
        if(write_size == -1) {
            perror("ERROR ASKING IN COURSE ID");
            _exit(0);
        }
        bzero(buff, sizeof(buff));
        bzero(read_buff, sizeof(read_buff));
        read(desc, read_buff, sizeof(read_buff));
        if(buff[0] == '^'){
            continue;
        }
        if(isalpha(read_buff)){
                write(desc,"Wrong id entered, Try again.^",sizeof("Wrong id entered, Try again.^"));
        }
        else{
            cid = atoi(read_buff);
            if(cid<=0 || get_count(3)<cid){
                write(desc,"Wrong id entered, Try again.^",sizeof("Wrong id entered, Try again.^"));
            }
            else
                break;
        }
    }
    int flag = 0; // Flag to indicate if the number is found
    for (int i = 0; i < cnt; i++) {
        if (enrolled[i] == cid) {
            flag = 1;
            break; // Number is found, exit the loop
        }
    }
    if(flag == 0){
        write(desc,"Course not Enrolled.^",sizeof("Course not Enrolled.^"));
        return ;
    }
    else{
        struct enroll e = get_student_enrollment(desc,id,cid);
        int id = e.enroll_id;
        lock.l_type = F_WRLCK;
        lock.l_whence = SEEK_SET;
        lock.l_start = (id-1)*sizeof(struct enroll);
        lock.l_len = sizeof(struct enroll);
        lock.l_pid = getpid();
        int f = fcntl(fd, F_SETLKW, &lock);
        if(f==-1){
            perror("Error obtaining write lock on Enrollment Record!");
            close(fd);
        }
        if(e.is_enrolled){
            set_availability(desc,id,1);
            e.is_enrolled=0;
            lseek(fd,(id-1)*sizeof(struct enroll),SEEK_SET);
            write(fd,&e,sizeof(struct enroll));
        }      
    }
    lock.l_type = F_UNLCK;
    fcntl(fd,F_SETLK,&lock);
    close(fd);
}


void change_password_student(int desc, int id) {}

void stud_login_handler (int desc) {
    struct prof professor;
    struct stud student;
    ssize_t write_size, read_size;
    char read_buff[1024], write_buff[1024];
    bzero(write_buff, 1024);
    bzero(read_buff, 1024);
    write_size = write(desc, ASK_LOGIN, sizeof(ASK_LOGIN));
    if(write_size == -1) {
        perror("CANNOT ASK LOGIN ID");
        _exit(0);
    }
    read_size = read(desc, read_buff, sizeof(read_buff));
    if(read_size == -1) {
        perror("CANNOT READ LOGIN ID");
        _exit(0);
    }
    int id = atoi(read_buff);
    int cnt;
    if(login(false, false, desc, id)) {
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
                    enroll_new_course(desc, id);
                    break;
                case 3:
                    drop_course(desc, id);
                    break;
                case 4:
                    view_enrolled_course_details(desc, id, &cnt, 1);
                    break;
                case 5:
                    change_password_student(desc, id);
                    break;
                default:
                    write(desc, LOGOUT_MSG, sizeof(LOGOUT_MSG));
                    exit(EXIT_SUCCESS);
            }
        }
    }
}