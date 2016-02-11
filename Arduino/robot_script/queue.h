#ifndef QUEUE_H
#define QUEUE_H

template<class T>
class Queue
{
    public:
        Queue();
        bool isEmpty();
        void enqueue(T data);
        void dequeue();


    private:
        struct Node{
            T node_data;
            Node *next;
        };

        Node *frontPtr;
        Node *backPtr;
        int count;

};

template<class T>
Queue<T>::Queue(): frontPtr(0), backPtr(0), count(0)
{
}

template<class T>
bool Queue<T>::isEmpty(){
    return(count == 0);
}

template<class T>
void Queue<T>::enqueue(T data){
    Node *newOne = new Node;
    newOne->node_data = data;
    newOne->next = 0;
    if(isEmpty()){
        frontPtr = newOne;
    }
        else{
            backPtr->next = newOne;
        }
        backPtr = newOne;
        count++;
}

template<class T>
void Queue<T>::dequeue(){
    if(isEmpty()){
        return;
    }
        else{
            Node *temp = frontPtr;
            if(frontPtr == backPtr){
                frontPtr = 0;
                backPtr = 0;
            }
            else{
                frontPtr = frontPtr->next;
            }
            delete temp;
            count--;
        }
}

#endif // QUEUE_H
