#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

struct Employee {
    char* name;
    double salary;
    char* gender;
};

struct EmployeeList {
    struct Employee* employee;
    struct EmployeeList* next;
};

typedef struct Employee* (*search_f) (struct Employee* list, void* arg);
typedef int (*delete_rule) (struct Employee* elem);

struct Employee* print_employee(struct Employee* employee, void* arg);
int delete_by_22(struct Employee* employee);
void print_list(struct EmployeeList* list);
struct Employee* create_employee(char* name, double salary, char* gender);
struct EmployeeList* add_element(struct EmployeeList* tail, struct Employee* employee);
struct EmployeeList* create_element(struct Employee* element_employee);
void delete_elem(struct EmployeeList** list, delete_rule should_delete);
struct Employee* cycle_list(struct EmployeeList *list, search_f filter, void* arg);
struct Employee* search_list(struct EmployeeList *list, search_f filter);
struct Employee* write_employee_to_file(struct Employee* employee, void* arg);

void save_list(struct EmployeeList* list, char* file_name) {
    FILE* fp = fopen(file_name, "wb");

    cycle_list(list, write_employee_to_file, fp);

    fclose(fp);

    fp = fopen(file_name, "wb");

    if (fp != NULL) {

        char buf[10];
        while (!feof(fp)) {
            fread(buf, 1, 6, fp);
            printf("%s\n", buf);
        }

        fclose(fp);
    }

}

struct Employee* write_employee_to_file(struct Employee* employee, void* arg) {

    FILE* file = arg;

    fwrite(employee->name, sizeof(char), strlen(employee->name), file);

    return NULL;
}

double filtered_sum(char filter_name, char* gender) {

}

int get_max_salary() {

}

struct Employee* search_list(struct EmployeeList *list, search_f filter) {

    struct EmployeeList* node = list;

    while (node != NULL) {

        if (filter != NULL) {
            return filter(node->employee, NULL);
        }

        node = node->next;
    }

}

struct Employee* cycle_list(struct EmployeeList *list, search_f filter, void* arg) {

    struct EmployeeList* node = list;

    while (node != NULL) {

        if (filter != NULL) {
            filter(node->employee, arg);
        }

        node = node->next;
    }

}

void delete_elem(struct EmployeeList** list, delete_rule should_delete) {

    struct EmployeeList* to_delete = NULL;

    struct EmployeeList* node = NULL;
    struct EmployeeList* node_next = *list;

    while (node_next != NULL) {

        if (should_delete(node_next->employee)) {
            to_delete = node_next;

            if (node == NULL) {

                node_next = node_next->next;
                *list = node_next;
            } else {

                node_next = node_next->next;
                node->next = node_next;

            }

            free(to_delete->employee);
            free(to_delete);
            continue;
        }

        node = node_next;
        node_next = node_next->next;
    }

}

struct EmployeeList* create_element(struct Employee* element_employee) {
    struct EmployeeList* element = (struct EmployeeList*) calloc(1, sizeof(struct EmployeeList));

    element->employee = element_employee;

    return element;
}

struct EmployeeList* add_element(struct EmployeeList* tail, struct Employee* employee) {
    struct EmployeeList* element = create_element(employee);

    element->next = NULL;

    tail->next = element;

    return element;
}

struct Employee* create_employee(char* name, double salary, char* gender) {
    struct Employee* employee = (struct Employee*) calloc(1, sizeof(struct Employee));
    
    employee->name = name;
    employee->salary = salary;
    employee->gender = gender;

    return employee;
}

void print_list(struct EmployeeList* list) {
    cycle_list(list, print_employee, NULL);
}

struct Employee* print_employee(struct Employee* employee, void* arg) {
    printf("%s %f %s\n", employee->name, employee->salary, employee->gender);

    return employee;
}

int delete_by_22(struct Employee* employee) {
    if (employee->salary == 22) {
        return 1;
    } else {
        return 0;
    }
}

int main() {
    struct EmployeeList* tail;

    struct Employee *employee1 = create_employee("Pesho", 22, "male");
    struct EmployeeList* head = create_element(employee1);

    tail = add_element(head, create_employee("gosho", 23, "what"));
    tail = add_element(tail, create_employee("gosho", 24, "what"));
    tail = add_element(tail, create_employee("gosho", 25, "what"));

    print_list(head);

    delete_elem(&head, delete_by_22);

    printf("\n");

    print_list(head);

    save_list(head, "employees");

    return 0;

}
