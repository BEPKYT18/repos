#include <iostream>
#include <string>
#include <fstream>
#include <windows.h>
#include <sstream>
#include <cstdint>

struct Student_name {
    int num;
    std::string name;
    std::string surname;
    std::string patr;
};

struct Student_marks {
    int group, num;
    std::string subj[3];
    int marks[3];
};

struct Student {
    std::string surname;
    int group, num;
    std::string subj[3];
    int marks[3];
};

//задание а
void check_file(std::ifstream& file) {
    if (!file.is_open()) throw "file error open";
    if (file.peek() == EOF) throw "file error empty";
}

void check_open_file(std::ofstream& file) {
    if (!file.is_open()) throw "file error open";
}

std::string read_without_puncts(std::ifstream& file, const std::string& delim) {
    std::string line, result;
    while (std::getline(file, line)) {
        for (char c : line) {
            if (delim.find(c) == std::string::npos) {
                result += c;
            }
            else result += " ";
        }
        result += '\n';
    }
    return result;
}

int return_size(std::ifstream& file) {
    int total{};
    std::string line;

    while (std::getline(file, line)) ++total;
    file.clear();
    file.seekg(0);

    return total;
}

Student_name* fill_mass1(std::ifstream& file, int size) {
    std::string delim = ",.;:?";
    std::string text = read_without_puncts(file, delim);

    Student_name* mass = new Student_name[size]{};
    std::istringstream iss(text);

    int book_num{};
    std::string w1, w2, w3;

    int temp = 0;
    while ((temp < size) && (iss >> book_num >> w1 >> w2 >> w3)) {
        mass[temp].num = book_num;
        mass[temp].name = w1;
        mass[temp].surname = w2;
        mass[temp].patr = w3;
        ++temp;
    }

    return mass;
}

Student_marks* fill_mass2(std::ifstream& file, int size) {
    std::string delim = ",.;:?";
    std::string text = read_without_puncts(file, delim);

    Student_marks* mass = new Student_marks[size]{};
    std::istringstream iss(text);

    int group_num{}, book_num{};
    int mark0{}, mark1{}, mark2{};
    std::string w0, w1, w2;

    int temp = 0;
    while ((temp < size) && (iss >> group_num >> book_num >>
                             w0 >> mark0 >> w1 >> mark1 >> w2 >> mark2)) {
        mass[temp].group = group_num;
        mass[temp].num = book_num;

        mass[temp].marks[0] = mark0;
        mass[temp].marks[1] = mark1;
        mass[temp].marks[2] = mark2;

        mass[temp].subj[0] = w0;
        mass[temp].subj[1] = w1;
        mass[temp].subj[2] = w2;
        ++temp;
    }

    return mass;
}

void write_str(std::ofstream& file, const std::string& s) {
    uint32_t len = static_cast<uint32_t>(s.size());
    file.write(reinterpret_cast<const char*>(&len), sizeof(len));
    file.write(s.data(), len);
}

bool read_str(std::ifstream& file, std::string& s) {
    uint32_t len = 0;
    file.read(reinterpret_cast<char*>(&len), sizeof(len));
    if (!file) return {};
    s.resize(len);
    return static_cast<bool>(file.read(s.data(), len));
}

void write_student_name(std::ofstream& out, const Student_name& s) {
    out.write(reinterpret_cast<const char*>(&s.num), sizeof(s.num));
    write_str(out, s.name);
    write_str(out, s.surname);
    write_str(out, s.patr);
}

bool read_student_name(std::ifstream& in, Student_name& s) {
    if (!in.read(reinterpret_cast<char*>(&s.num), sizeof(s.num))) return false;
    if (!read_str(in, s.name)) return false;
    if (!read_str(in, s.surname)) return false;
    if (!read_str(in, s.patr)) return false;
    return true;
}

void write_student_marks(std::ofstream& out, const Student_marks& s) {
    out.write(reinterpret_cast<const char*>(&s.group), sizeof(s.group));
    out.write(reinterpret_cast<const char*>(&s.num), sizeof(s.num));

    for (int i{}; i < 3; ++i) {
        write_str(out, s.subj[i]);
    }

    out.write(reinterpret_cast<const char*>(&s.marks), sizeof(s.marks));
}

bool read_student_marks(std::ifstream& in, Student_marks& s) {
    if (!in.read(reinterpret_cast<char*>(&s.group), sizeof(s.group))) return false;
    if (!in.read(reinterpret_cast<char*>(&s.num), sizeof(s.num))) return false;

    for (int i{}; i < 3; ++i) {
        if (!read_str(in, s.subj[i])) return false;
    }

    if (!in.read(reinterpret_cast<char*>(&s.marks), sizeof(s.marks))) return false;
    return true;
}

void fill_bin_file1(std::ofstream& out, Student_name* mass, int size) {
    for (int i{}; i < size; ++i) {
        write_student_name(out, mass[i]);
    }
}

void fill_bin_file2(std::ofstream& out, Student_marks* mass, int size) {
    for (int i{}; i < size; ++i) {
        write_student_marks(out, mass[i]);
    }
}

Student_name* bin_to_str1(std::ifstream& in, int size) {
    Student_name* s = new Student_name[size]{};
    int i{};
    while ((i < size) && (read_student_name(in, s[i]))) {
        ++i;
    }
    return s;
}

Student_marks* bin_to_str2(std::ifstream& in, int size) {
    Student_marks* s = new Student_marks[size]{};
    int i{};
    while ((i < size) && (read_student_marks(in, s[i]))) {
        ++i;
    }
    return s;
}

void print_struct1(Student_name* s, int size) {
    for (int i{}; i < size; ++i) {
        std::cout << s[i].num << " " << s[i].name << " " << s[i].surname
        << " " << s[i].patr << " " << '\n';
    }
}

void print_struct2(Student_marks* s, int size) {
    for (int i{}; i < size; ++i) {
        std::cout << s[i].group << " " << s[i].num << " ";
        std::cout << s[i].subj[0] << " " << s[i].marks[0] << " ";
        std::cout << s[i].subj[1] << " " << s[i].marks[1] << " ";
        std::cout << s[i].subj[2] << " " << s[i].marks[2] << " " << '\n';
    }
}

//задание б
Student* fill_student(Student_name* s1, Student_marks* s2, int size) {
    Student* s = new Student[size]{};
    for (int i{}; i < size; ++i) {
        for (int j{}; j < size; ++j) {
            if (s1[i].num == s2[j].num) {
                s[i].surname = s1[i].surname;
                s[i].group = s2[j].group;
                s[i].num = s1[i].num;
                for (int j1{}; j1 < 3; ++j1) {
                    s[i].subj[j1] = s2[j].subj[j1];
                }
                for (int j2{}; j2 < 3; ++j2) {
                    s[i].marks[j2] = s2[j].marks[j2];
                }
            }
        }
    }
    return s;
}

void write_student(std::ofstream& out, const Student& s) {
    write_str(out, s.surname);

    out.write(reinterpret_cast<const char*>(&s.group), sizeof(s.group));
    out.write(reinterpret_cast<const char*>(&s.num), sizeof(s.num));

    for (int i{}; i < 3; ++i) {
        write_str(out, s.subj[i]);
    }

    out.write(reinterpret_cast<const char*>(&s.marks), sizeof(s.marks));
}

void fill_bin_file(std::ofstream& out, Student* mass, int size) {
    for (int i{}; i < size; ++i) {
        write_student(out, mass[i]);
    }
}

//задание в
double avg(int* mass, int len) {
    double total{};
    for (int i{}; i < len; ++i) total += mass[i];
    return total / len;
}

void fill_bin_file_with_avg(std::ofstream& out, Student* mass, int size) {
    int len = sizeof(mass[0].marks) / sizeof(mass[0].marks[0]);
    for (int i{}; i < size; ++i) {
        write_student(out, mass[i]);
        double avg_marks = avg(mass[i].marks, len);
        out.write(reinterpret_cast<const char*>(&avg_marks), sizeof(avg_marks));
    }
}


int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    std::ifstream fin1("students_name.txt");
    try {check_file(fin1);}
    catch(const char* msg) {std::cout << msg; return 1;}

    std::ifstream fin2("students_marks.txt");
    try {check_file(fin2);}
    catch(const char* msg) {std::cout << msg; return 1;}

    int size = return_size(fin1);

    std::ofstream fout1("students_name.bin", std::ios::binary);
    try {check_open_file(fout1);}
    catch(const char* msg) {std::cout << msg; return 1;}

    std::ofstream fout2("students_marks.bin", std::ios::binary);
    try {check_open_file(fout2);}
    catch(const char* msg) {std::cout << msg; return 1;}

    Student_name* names = fill_mass1(fin1, size);
    Student_marks* marks = fill_mass2(fin2, size);

    fill_bin_file1(fout1, names, size);
    fill_bin_file2(fout2, marks, size);

    fout1.close(); fout2.close();

    delete[] names; delete[] marks;

    std::ifstream fin_bin1("students_name.bin", std::ios::binary);
    try {check_file(fin_bin1);}
    catch(const char* msg) {std::cout << msg; return 1;}

    std::ifstream fin_bin2("students_marks.bin", std::ios::binary);
    try {check_file(fin_bin2);}
    catch(const char* msg) {std::cout << msg; return 1;}

    Student_name* stud_name = bin_to_str1(fin_bin1, size);
    Student_marks* stud_marks = bin_to_str2(fin_bin2, size);

    print_struct1(stud_name, size);
    print_struct2(stud_marks, size);

    fin_bin1.close(); fin_bin2.close();

    std::ofstream fout("stud.bin", std::ios::binary);
    try {check_open_file(fout);}
    catch(const char* msg) {std::cout << msg; return 1;}

    Student* student = fill_student(stud_name, stud_marks, size);
    fill_bin_file(fout, student, size);
    fout.close();

    std::ofstream fout_avg("stud_with_avg.bin", std::ios::binary);
    try {check_open_file(fout_avg);}
    catch(const char* msg) {std::cout << msg; return 1;}

    fill_bin_file_with_avg(fout_avg, student, size);
    fout_avg.close();

    delete[] stud_name;
    delete[] stud_marks;
    delete[] student;

    return 0;
}
