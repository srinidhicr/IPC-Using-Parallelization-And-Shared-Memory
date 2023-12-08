#include <iostream>
#include <numeric>
#include <vector>
#include <fstream>
#include <string.h>
#include <omp.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <cstdlib>
#include <iomanip>
#include <ios>
using namespace std;

void segmentWrite()
{
        // ftok to generate unique key
        key_t key = ftok("shmfile", 65);

        // shmget returns an identifier in shmid
        int shmid = shmget(key, 1024, 0666 | IPC_CREAT);

        // shmat to attach to shared memory
        char *str = (char*) shmat(shmid, (void*)0 ,0);
        cout << setw(50);
        cout <<  "-- DATA TO BE WRITTEN INTO MEMORY --\n";
        cout << setw(35);
        cout << "Enter data - ";
        cin >> str;

        //detach from shared memory
        shmdt(str);
}

void segmentRead()
{
        // ftok to generate unique key
        key_t key = ftok("shmfile", 65);

        // shmget returns an identifier in shmid
        int shmid = shmget(key, 1024, 0666 | IPC_CREAT);

        // shmat to attach to shared memory
        char *str = (char*) shmat(shmid, (void*)0, 0);
        cout << " -- DATA CURRENTLY IN MEMORY --\n\n";
        cout << str;
        //detach from shared memory
        shmdt(str);
}

void segmentUpdate()
{
    // Generate unique key
    key_t key = ftok("shmfile", 65);

    // Get identifier for shared memory segment
    int shmid = shmget(key, 1024, 0666 | IPC_CREAT);

    // Attach shared memory segment to current process
    char *str = (char*) shmat(shmid, (void*) 0, 0);

    string s, new_data, data;

    int option;
    do {
        cout << setw(65);
        cout << " -- INTER PROCESS COMMUNICATION -- \n\n";
        cout << setw(50);
        cout << "1. Append data\n";
        cout << setw(49);
        cout << "2. Clear data\n";
        cout << setw(48);
        cout << "3. View data\n";
        cout << setw(51);
        cout << "4. Replace data\n";
        cout << setw(46);
        cout << "5. Execute\n";
        cout << setw(49);
        cout << "6. Word Count\n";
        cout << setw(49);
        cout << "7. Line count\n";
        cout << setw(50);
        cout << "8. Word Search\n";
        cout << setw(51);
        cout << "9. To Uppercase\n";
        cout << setw(52);
        cout << "10. To Lowercase\n";
        cout << setw(44);
        cout << "11. Exit\n";
        cout << "\nEnter option: ";
        cin>>option;
        cout<<endl;

        switch (option) {
            case 1: {
                system("clear");
                cout << "-- APPEND DATA --\n\n";
                cout << "Enter data to be appended (press '.' on a new line to finish) \n";
                cin.ignore();
                data = "";
                while (getline(cin, s)) {
                    if (s == ".")
                        break;
                    data += s + "\n";
                }
                strcat(str, data.c_str());
                cout << endl << endl;
                system("clear");
                break;
            }
            case 2:
                cout<<endl;
                system("clear");
                cout << "-- CLEAR DATA --\n\n";
                memset(str, 0, strlen(str));
                cout << "Data cleared from memory\n\n";
                break;

            case 3:
                system("clear");
                cout << "-- VIEW DATA --\n\n";
                segmentRead();
                break;

            case 4: {
                system("clear");
                cout<<endl;
                cout << "-- REPLACE DATA --\n\n";
                memset(str, 0, strlen(str));
                cout << "Enter data to be replaced (press '.' on a new line to finish) \n";
                cin.ignore();
                data = "";
                while (getline(cin, s)) {
                    if (s == ".")
                        break;
                    data += s + "\n";
                }
                strcat(str, data.c_str());
                cout << endl;
                break;
            }

            case 5: {
                system("clear");
                cout << "-- RUN SEGMENT --\n\n";
                ofstream script("run.sh");
                script << str;
                script.close();
                system("bash run.sh");
                cout << endl;
                break;
            }

            case 6: {
                system("clear");
                cout<<endl;
                cout << "-- WORD COUNT --\n\n";
                int size = strlen(str);
                int count = 0;
                for (int i = 0; i < size; i++) {
                    if (isspace(str[i])) {
                        count++;
                    }
                }
                cout << "Number of words in memory: " << count << '\n';
                cout << endl << endl;
                break;
            }
            case 7: {
                cout<<endl;
                system("clear");
                cout << endl;
                cout << "-- LINE COUNT --\n\n";
                int size = strlen(str);
                int count = 0;
                for (int i = 0; i < size; i++) {
                    if (str[i] == '\n') {
                        count++;
                    }
                }
                cout << "Number of lines in memory: " << count << '\n';
                cout << endl;
                break;
            }
            case 8: {
                cout<<endl;
                string s;
                system("clear");
                cout << endl;
                cout << "-- WORD SEARCH --\n\n";
                cout << "Enter a search string: ";
                cin.ignore();
                getline(cin, s);
                char *found = strstr(str, s.c_str());
                if (found != NULL) {
                    cout << "Search string found in memory at position " << found - str << '\n';
                } else {
                    cout << "Search string not found in memory\n";
                }
                cout << endl << endl;
                break;
            }
            case 9: {
                cout<<endl;
                system("clear");
                cout << "-- TO UPPER-CASE --\n\n";
                int size = strlen(str);
                for (int i = 0; i < size; i++) {
                    if (isalpha(str[i])) {
                        str[i] = toupper(str[i]);
                    }
                }
                cout << "Data in memory converted to upper case\n\n";
                break;
            }
            case 10: {
                cout<<endl;
                system("clear");
                cout << "-- TO LOWER-CASE --\n";
                int size = strlen(str);
                for (int i = 0; i < size; i++) {
                    if (isalpha(str[i])) {
                        str[i] = tolower(str[i]);
                    }
                }
                cout << "Data in memory converted to lower case\n\n";
                break;
            }
            case 11:
                system("clear");
                cout<<endl;
                break;

            default:
                system("clear");
                cout << "Invalid option selected\n";
                break;
        }
    } while (option != 11);
    // Detach shared memory segment from current process
    shmdt(str);
}

int main()
{
        system("clear");
        cout << "\033[36m";
        cout<<endl;
        int k;
        cout << setw(50);
        cout << " -- INTER PROCESS COMMUNICATION -- \n";
        cout << setw(50);
        cout << "--------------------------------\n";
        cout << setw(45);
        cout<<"Number of parellel processes :- ";
        cin>>k;
        system("clear");
        #pragma omp parallel num_threads(k)
        {
            int tid = omp_get_thread_num();
            #pragma omp critical
            {
                cout<<"\nCurrent Thread :- "<<tid << endl;
                // This section of code will be executed by exactly one thread at a time
                segmentUpdate();
            }
        }
        cout<<endl<<endl;
        cout << "\033[0m";
        return 0;
}
