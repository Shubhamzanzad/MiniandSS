struct prof {
    int prof_id;
    char prof_name[50];
    char prof_email[50];
    char course[50];
    int prof_age;
    // int course_id;
    // int course_type;
    char prof_password[50];
};

struct stud {
    int stud_id;
    //char stud_type[10];
    char stud_name[50];
    char stud_email[50];
    int stud_age;
    char stud_address[50];
    bool flag;
    //int courses[10];
    char stud_password[50];
    // struct courses {
    //     int course_id;
    //     char course_id[50];
    // };
};

struct admin {
    int id;
    char admin_password[5];
};

struct course {
    int course_id;
    int prof_id;
    int seats;
    int credit;
    char course_name[50];
    int available;
};

struct enroll {
    int stud_id;
    int course_id;
    int prof_id;
    int enroll_id;
    int is_enrolled;
};

struct record {
    int student;
    int teacher;
    int courses;
    int enrolls;
};

