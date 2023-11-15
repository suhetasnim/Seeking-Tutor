using namespace std;
#include<bits/stdc++.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
class Node
{
public:
    int student_id;
    int priority;
    int justseated;
    int sequence;
    Node *next;
    Node()
    {
        next= NULL;
    }


    Node(int key1, int key2, int key3, int key4)
    {
        student_id = key1;
        priority = key2;
        justseated = key3;
        sequence = key4;
        next= NULL;
    }

};

class List
{
public:
    Node *head, *tail;

public:
    List()
    {
        head= NULL;
        tail= NULL;
    }

    int size()
    {
        int count=0;
        Node *temp=head;
        if (temp==NULL)
            return 0;
        else if (temp->next==NULL)
            return 1;

        else
        {
            while(temp!=NULL)
            {
                count++;
                temp=temp->next;
            }

            return count;
        }
    }

    void Add(int element1, int element2, int element3, int element4)
    {
        Node *newItem = new Node(element1, element2, element3, element4);

        if(head == NULL)
        {
            head = newItem;
            tail = newItem;
        }
        else
        {
            tail->next= newItem;
            tail= newItem;
        }
    }

    void RemoveFirst()
    {
        if(head==NULL)
        {
            cout<< "This list is empty. You can not remove any elements\n" << endl;
        }
        else
        {
            Node *temp=head, *temp1=head->next;
            if(temp1==NULL)
            {
                delete(temp1);
                head=NULL;
                tail=NULL;
            }
            else
            {
                head=temp1;
                delete(temp);
            }
        }
    }

    void Erase()
    {
        int count=size();
        int position=1;
        while(position<=count)
        {
            RemoveFirst();
            position++;
        }
    }


    int get(int index)
    {
        Node *temp=head;
        int count=size();
        if (count==0)
            cout<<"This list is empty!"<<endl;
        else if(index>0 && index<=count)
        {
            for(int i=0; i<index-1; i++)
            {
                temp=temp->next;
            }
            return temp->student_id;
        }

        else
            cout <<"Invalid index!"<<endl;
    }


    int get_priority(int studentid)
    {
        Node *temp=head;
        int count=size();
        if (count==0)
            cout<<"this list is empty!"<<endl;
        else
        {
            for(int i=0; i<count; i++)
            {
                if(temp->student_id == studentid)
                {
                    return temp->priority;
                }
                temp=temp->next;
            }
        }
    }


    int get_justseated_value(int studentid)
    {
        Node *temp=head;
        int count=size();
        if (count==0)
            cout<<"This is get_justseated_value function. This list is empty!"<<endl;
        else
        {
            for(int i=0; i<count; i++)
            {
                if(temp->student_id == studentid)
                {
                    return temp->justseated;
                }
                temp=temp->next;
            }
        }
    }


    void update_values(int studentid, int justseated_value, int sequence_value)
    {
        Node *temp=head;
        int count=size();
        if (count==0)
            cout<<"This list is empty!"<<endl;
        else
        {
            for(int i=0; i<count; i++)
            {
                if(temp->student_id == studentid)
                {
                    temp->justseated = justseated_value;
                    temp->sequence = sequence_value;
                }
                temp=temp->next;
            }
        }
    }


    void update_values2(int studentid, int justseated_value, int updated_priority)
    {
        Node *temp=head;
        int count=size();
        if (count==0)
            cout<<"This list is empty!"<<endl;
        else
        {
            for(int i=0; i<count; i++)
            {
                if(temp->student_id == studentid)
                {
                    temp->justseated = justseated_value;
                    temp->priority = updated_priority;
                }
                temp=temp->next;
            }
        }
    }


    int getFirst()
    {
        int a = get(1);
        return a;
    }


    int getLast()
    {
        int a = get(size());
        return a;
    }

    void print()
    {
        Node* p = head;
        if(head==NULL)
        {
            cout << "List is empty!" << endl;
            return;
        }

        while(p!= NULL)
        {
            cout << "Student id = "<< p->student_id
                 << ", Priority = " << p->priority
                 << ", Just seated value = " << p->justseated
                 << ", Sequence = " << p->sequence << endl;

            p = p->next;
        }
        cout <<endl;
    }

};



int done=0;


int student_num=0;
int tutor_num=0;
int help_num=0;
int chair_num=0;


int occupied_chair_num=0;


List *list1=new List();

List *list3=new List();
List *list4=new List();



sem_t sem_student;
sem_t sem_coordinator;
sem_t m;

void printUpdate()
{
    printf("\n...........Printing all the updates...........\n");
    printf("Number of students who have used all the helps: %d\n",done);
    printf("Number of occupied chairs: %d\n",occupied_chair_num);
    printf("Number of students waiting: %d\n",occupied_chair_num);
    printf("Number of empty chairs %d\n\n",chair_num - occupied_chair_num);
}

int c = 0;
int sequence = 0;



void *student_thread(void *p)
{
    sem_wait(&m);
    cout <<endl;
    cout<<"Entering student thread"<<endl;
    list1->Add(c,help_num,-1,-1);
    cout<<"Student id: "<<c<<" has been created"<<endl;
    c++;
    int student_id = list1->getLast();
    int student_priority = list1->get_priority(student_id);

    cout <<endl;
    cout<<"Printing student_thread list: " <<endl;
    list1->print();
    cout<<"Exiting student thread!"<<endl<<endl;
    sem_post(&m);
    sleep(1);

    while(1)
    {
        sem_wait(&m);
        if(list1->get_priority(student_id) == 0)
        {
            cout<<"This is student thread"<<endl;
            cout<<"Student " <<student_id << " have used all of his help."<<endl;
            cout<<"Terminating student thread: "<<student_id<<endl<<endl;

            if(done==student_num)
            {
                sem_post(&sem_student);
            }
            sem_post(&m);

            pthread_exit(NULL);
        }


        if(occupied_chair_num>=chair_num)
        {
            printf("Student %d did not find any empty chair, will try again later\n",student_id);
            sem_post(&m);

            sleep(3);
            continue;
        }

        printf("...................Student %d have helps left and also found an empty chair.\n\n",student_id);
        printf("Before sitting: ");
        printUpdate();

        occupied_chair_num++;


        sequence++;
        list1->update_values(student_id, 1, sequence);

        cout<<"After sitting: ";
        printUpdate();
        cout<<"Student thread list is: "<<endl;
        list1->print();

        sem_post(&sem_student);
        sem_post(&m);

        while(list1->get_justseated_value(student_id)==1);
        sleep(1);
    }

}


void *tutor_thread(void *tutor_id)
{
    while(1)
    {
        sem_wait(&m);
        if(done==student_num)
        {
            sem_post(&m);
            pthread_exit(NULL);
        }
        sem_post(&m);

        sem_wait(&sem_coordinator);

        sem_wait(&m);

        if(list3->size() == 0)
        {
            sem_post(&m);
            continue;
        }

        cout<<"This is the tutor thread list is:" <<endl;
        list3->print();

        Node *temp=list3->head;
        int cnt=list3->size();
        int max = 0;
        if (cnt==0)
            cout<<"This tutor thread list is empty!"<<endl;
        else
        {
            for(int i=0; i<cnt; i++)
            {
                if(temp->priority >max && temp->justseated==1)
                {
                    max = temp->priority;
                }
                temp=temp->next;
            }
            temp = list3->head;
            list4->Erase();
            for(int i=0; i<cnt; i++)
            {
                if(temp->priority == max && temp->justseated==1)
                {
                    list4->Add(temp->student_id, temp->priority, temp->justseated, temp->sequence);
                }
                temp=temp->next;
            }

            cout<<"Priority list (with the seated students) is: "<<endl;
            list4->print();

            cnt = list4->size();
            if(cnt==0)
            {
                sem_post(&m);
                continue;
            }


            int min_s = sequence;
            temp = list4->head;
            Node *temp1 = temp;
            for(int i=0; i<cnt; i++)
            {
                if(temp->sequence < min_s)
                {
                    temp1=temp;
                    min_s = temp->sequence;
                }
                temp=temp->next;
            }
            cout<<"The student which needs to be tutored is: "<<temp1->student_id<<endl;



            int updated_priority = temp1->priority - 1;
            list1->update_values2(temp1->student_id, -1, updated_priority);
            list3->update_values2(temp1->student_id, -1, updated_priority);
            cout<< "Student : "<<temp1->student_id << " got help from a tutor" <<endl;

            occupied_chair_num--;
            if(updated_priority==0)
            {

                done++;
            }
            cout<<"\n\nAfter tutoring student: " << temp1->student_id << " the update is: "<<endl;
            printUpdate();
            cout<<endl<<endl;

        }
        sem_post(&m);
    }


}


void *coordinator_thread(void *p)
{
    while(1)
    {

        if(done == student_num)
        {
            for(int i=0; i<tutor_num; i++)
            {

                sem_post(&sem_coordinator);
            }
            cout<<"Terminating coordinator thread"<<endl << endl;
            pthread_exit(NULL);

        }


        sem_wait(&sem_student);
        sem_wait(&m);
        list3->Erase();
        Node *temp= list1->head;
        int count=list1->size();

        for(int i=0; i<count; i++)
        {
            if(temp->justseated == 1)
            {
                list3->Add(temp->student_id, temp->priority, temp->justseated, temp->sequence);
            }
            temp=temp->next;
        }

        cout<<"This is coordinator thread list is: " <<endl;
        list3->print();

        sem_post(&sem_coordinator);
        sem_post(&m);

    }
}



int main()
{

    cout<<"Enter number of students: ";
    cin>>student_num;
    cout<<"Enter number of tutors: ";
    cin>>tutor_num;
    cout<<"Enter number of times the student can get help: ";
    cin>>help_num;
    cout<<"Enter number of chairs in the waiting room: ";
    cin>>chair_num;

    sem_init(&sem_student,0,0);
    sem_init(&sem_coordinator,0,0);
    sem_init(&m,0,1);


    pthread_t students[student_num];
    pthread_t tutors[tutor_num];
    pthread_t coordinator;


    if((pthread_create(&coordinator,NULL,coordinator_thread,NULL)) < 0)
    {
        perror("Error: coordinator thread cannot be created\n");
        exit(1);
    }

    for(int i = 0; i < student_num; i++)
    {
        pthread_create(&students[i], NULL, student_thread, (void*)i);
    }


    for(int j = 0; j < tutor_num; j++)
    {
        if((pthread_create(&tutors[j], NULL, tutor_thread, (void*) j)) < 0)
        {
            perror("Error: tutor thread cannot be created\n");
            exit(1);
        }
    }


    pthread_join(coordinator, NULL);

    for(int k =0; k < student_num; k++)
    {
        pthread_join(students[k],NULL);
    }
    for(int m =0; m < tutor_num; m++)
    {
        pthread_join(tutors[m],NULL);
    }
    cout<<"End of main function"<<endl<<endl;
    printUpdate();
    cout<<endl<<endl;
    return 0;
}

