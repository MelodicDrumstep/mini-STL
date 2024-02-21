#ifndef SJTU_AVL
#define SJTU_AVL

#include <cstddef>
#include <cmath>
#include <iostream>

using namespace std;

struct BSTnode
{
    long long val;
    BSTnode * left, *right;
    int height;
    BSTnode()
    {
        val = -1;
        left = right = NULL;
        height = 1;
    }
    BSTnode(long long v)
    {
        val = v;
        left = right = NULL;
        height = 1;
    }
    //We define the balance factor as the height of the
    //right subtree minus the height of the left subtree
    //I initially just restore the balance factor in the BSTnode
    //But then I find it somewhat difficult to implement the
    //delete function thus I turn to this version
}; 

class BST
{
public:
    BST()
    {
        root = NULL;
    }
    BST(BST & other)
    {
        if(other.root == NULL)
        {
            //cout << "OK" << endl;
            root = NULL;
            return;
        }
        root = NULL;
        copy(root, other.root);
    }
    BSTnode * root;
    //I forget to write this constructor and lead to some bugs 
private:
    BSTnode * copy(BSTnode *& n1, BSTnode *& n2)
    {
        if(n2 == NULL)
        {
           // cout << "OK1" << endl;
            return NULL;
        }
         //  cout << "OK2" << endl;
        n1 = new BSTnode(n2 -> val);
       // cout << "NUM  " << n2 -> val << endl; 
        n1 -> left = copy(n1 -> left, n2 -> left);
        n1 -> right = copy(n1 -> right, n2 -> right);
        update(n1);
        return n1;
    }
    void update(BSTnode * node)
    {
        if(node -> left == NULL && node -> right == NULL)
        {
            node -> height = 1;
            return;
        }
        if(node -> left == NULL)
        {
            node -> height = node -> right -> height + 1;
            return;
        }
        if(node -> right == NULL)
        {
            node -> height = node -> left -> height + 1;
            return;
        }
        node -> height = max(node -> left -> height, node -> right -> height) + 1;
    }
    int getheight(BSTnode * node)
    {
        //Note that this function is crucial otherwise we would 
        //have some runtime error when tring to get a height
        //from a NULL pointer
        if(node == NULL)
        {
            return 0;
        }
        return node -> height;
    }
    int getbf(BSTnode * node)
    {
        return getheight(node -> right) - getheight(node -> left);
    }
public:
    BSTnode * find(long long v)
    {
        BSTnode * answer = root;
        if(root == NULL)
        {
            return NULL;
        }
        //cout << root -> val << endl;
        while(answer != NULL)
        {
            if(answer -> val == v)
            {
                return answer;
            }
            if(v > answer -> val)
            {
                //cout << v << "  " << answer -> val << endl;
                if(answer -> right == NULL)
                {
                     //cout << "OK1" << endl;
                    return NULL;
                }
                answer = answer -> right;
            }
            else
            {
                if(answer -> left == NULL)
                {
                     //cout << "OK2" << endl;
                    return NULL;
                }
                answer = answer -> left;
            }
        }
        // cout << "OK3" << endl;
        return NULL;
    }
    BSTnode * smallest()
    {
        BSTnode * result = root;
        if(result == NULL)
        {
            return NULL;
        }
        while(result -> left)
        {
            result = result -> left;
        }
        return result;
    }
    BSTnode * largest()
    {
        BSTnode * result = root;
        if(result == NULL)
        {
            return NULL;
        }
        while(result -> right)
        {
            result = result -> right;
        }
        return result;
    }
    int range(BSTnode * n, int l, int r)
    {
        if(n == NULL)
        {
            return 0;
        }
        if(n -> val == l && n -> val == r)
        {
            return 1;
        }
        if(n -> val <= r && n -> val >= l)
        {
            return 1 + range(n -> left, l, r) + range(n -> right, l, r);
        }
        else if(n -> val < l)
        {
            return range(n -> right, l, r);
        }
        else
        {
            return range(n -> left, l, r);
        }
    }
    BSTnode * pred(BSTnode * node)
    {
        if(node -> left == NULL)
        {
            BSTnode * temp = root;
            BSTnode * last = root;
            while(temp -> val != node -> val)
            {
                if(node -> val > temp -> val)
                {
                    last = temp;
                    temp = temp -> right;
                }
                else
                {
                    temp = temp -> left;
                }
            }
            return last;
        }
        BSTnode * result = node -> left;
        while(result -> right)
        {
            result = result -> right;
        }
        return result;
    }
    BSTnode * succ(BSTnode * node)
    {
        if(node -> right == NULL)
        {
            BSTnode * temp = root;
            BSTnode * last = root;
            while(temp -> val != node -> val)
            {
                if(node -> val > temp -> val)
                {
                    temp = temp -> right;
                }
                else
                {
                    last = temp;
                    temp = temp -> left;
                }
            }
            return last;
        }
        BSTnode * result = node -> right;
        while(result -> left)
        {
            result = result -> left;
        }
        return result;
    }
    void insert(long long v, bool & hasbeen)
    {
        if(root == NULL)
        {
            root = new BSTnode;
            root -> left = root -> right = NULL;
            root -> val = v;
            root -> height = 1;
            //cout << "ALL right";
            return;
        }
        insertAVL(root, v, hasbeen);
        //We want to have a function which returns an
        //pointer to provide information when recursing
        //Thus we encapsulate one
    }
    void delete_node(long long v)
    {
        delete_AVL(root, v);
    }
private:
    BSTnode * insertAVL(BSTnode * t, long long v, bool & hasbeen)
    {
        // if(root == NULL)
        // {
        //     root = new BSTnode;
        //     root -> left = root -> right = NULL;
        //     root -> val = v;
        //     root -> height = 1;
        //     //cout << "ALL right";
        //     return root;
        // }
        if(t == NULL)
        {
            BSTnode *temp = new BSTnode;
            temp -> left = temp -> right = NULL;
            temp -> height = 1;
            temp -> val = v;
            //cout << " P " << endl;  
            return temp;
        }
        if(v < t -> val)
        {
            t -> left = insertAVL(t -> left, v, hasbeen);
        }
        else if(v > t -> val)
        {
            t -> right = insertAVL(t -> right, v, hasbeen);
            // cout << "OK";
        }
        else if(v == t -> val)
        {
            hasbeen = 1;
            return t;
        }
        update(t);
        int bf = getbf(t);
        if(bf == -2 && v < t -> left -> val)
        //This means a problem occurs in the left subtree of the left
        //subtree of the t node, thus we have to perform a right rotation
        {
            return rotate_right(t);
        }
        if(bf == 2 && v > t -> right -> val)
        //This is the left rotation case
        {
            return rotate_left(t);
        }
        if(bf == -2 && v > t -> left -> val)
        //This is the left right rotation case
        {
            t -> left = rotate_left(t -> left);
            return rotate_right(t);
        }
        if(bf == 2 && v < t -> right -> val)
        //This is the right left rotation case
        {
            t -> right = rotate_right(t -> right);
            return rotate_left(t);
        }
        return t;
    }
    BSTnode * rotate_left(BSTnode * t)
    {
        if(root == t)
        {
            root = t -> right;
        }
        //I forget to write this stuff which led to some bugs
        //because I have to change the root node if I perform rotation
        //about the root node.
        BSTnode * t1 = t -> right;
        BSTnode * t2 = t1 -> left;
        t -> right = t2;
        t1 -> left = t;
        t = t1;
        update(t -> left);
        update(t);
        return t;
    }
    BSTnode * rotate_right(BSTnode *  t)
    {
        if(root == t)
        {
            root = t -> left;
        }
        BSTnode * t1 = t -> left;
        BSTnode * t2 = t1 -> right;
        t -> left = t2;
        t1 -> right = t;
        t = t1;
        update(t -> right);
        update(t);
        return t;
    }
    public:
    BSTnode * delete_AVL(BSTnode * t, long long v)
    {
        //cout << "DD" << endl;
        //cout << t -> val << endl;
        if(t == NULL)
        {
            //cout << "OK!!" << endl;
            return t;
        }
        if(v < t -> val)
        {
            //cout << "L" << endl;
            t -> left = delete_AVL(t -> left, v);
        }
        else if(v > t -> val)
        {
            t -> right = delete_AVL(t -> right, v);
        }
        else
        {
            if(t -> left == NULL && t -> right == NULL)
            {
                //This is the case when the node has no child
                delete t;
                //cout << "ok" << endl;
                return NULL;
            }
            else if(t -> right == NULL)
            {
                BSTnode * temp = t -> left;
                delete t;
                t = temp;
            }
            else if(t -> left == NULL)
            {
                BSTnode * temp = t -> right;
                delete t;
                t = temp;
            }
            else
            //This is the two child case which is the most complicated
            {
                BSTnode * sub = t -> left;
                while(sub -> right)
                {
                    sub = sub -> right;
                }
                //We just find the rightest node in the left
                //subtree of t node
                //And it will be the successor of the node t
                t -> val = sub -> val;
                t -> left = delete_AVL(t -> left, t -> val);
                //We just use the delete function to delete
                //the node which has been reduced to the situations 
                //above
            }
        }
        if(t == NULL)
        {
            //cout << "here" << endl;
            return t;
        }
        //cout << "Here" << endl;
        //Now it's similar to the insertion situation
        update(t);
        int bf = getbf(t);
        //cout << "BF:: " << bf << endl;
        if(bf == -2 && getbf(t -> left) <= 0)
        //This means a problem occurs in the left subtree of the left
        //subtree of the t node, thus we have to perform a right rotation
        {
            return rotate_right(t);
        }
        if(bf == 2 && getbf(t -> right) >= 0)
        //This is the left rotation case
        {
            //cout << "LEFT" << endl;
            return rotate_left(t);
        }
        if(bf == -2 && getbf(t -> left) > 0)
        //This is the left right rotation case
        {
            t -> left = rotate_left(t -> left);
            return rotate_right(t);
        }
        if(bf == 2 && getbf(t -> right) < 0)
        //This is the right left rotation case
        {
            t -> right = rotate_right(t -> right);
            return rotate_left(t);
        }
        return t;
    }
public:
    
};

struct node
{
    node()
    {
        array = new BST;
        next = NULL;
    }
    node(node & other)
    {
        array = new BST(*(other.array));
        next = NULL;
    }
    BST * array;
    node * next;
};

node * head;
node* now_tree = NULL;
node * now;
long long now_val = -1;
long long temp;
bool test;
long long v1;
BSTnode * r1;
node * tail;

void add(int i, long long v)
{
    now = head;
    for(int k = 0; k < i; k++)
    {
        now = now -> next;
    }
    if(now -> array -> find(v) != NULL)
    {
        return;
    }
    test = 0;
    now -> array -> insert(v, test);
    now_tree = now;
    now_val = v;
        //cout << "NOW ::" << now_val << endl;
    //cout << "NOW ::" << now -> array -> root -> val << endl;
}

void remove(int i, long long v)
{
    now = head;
    for(int k = 0; k < i; k++)
    {
        now = now -> next;
    }
    if(now -> array -> root == NULL)
    {
        return;
    }
    r1 = now -> array -> find(v);
    if(r1 != NULL)
    {
        //cout << 1 << endl;
        if(now_tree == now && now_val == v)
        {
            now_tree = NULL;
        }
        now -> array -> root = now -> array -> delete_AVL(now -> array -> root, v);
    }
        //cout << "NOW ::" << (now -> array -> root == NULL) << endl;
}

bool query(int i, long long v2)
{
    now = head;
    for(int k = 0; k < i; k++)
    {
        now = now -> next;
    }
    r1 = now -> array -> find(v2);
    if(r1 != NULL)
    {
        now_tree = now;
        now_val = v2;
        return true;
    }
    else
    {
        return false;
    }
}

void fork(int i)
{
    now = head;
    for(int k = 0; k < i; k++)
    {
        now = now -> next;
    }
    tail = head;
    while(tail -> next != NULL)
    {
        tail = tail -> next;
    }
    tail -> next = new node(*now);
}

int range(int i, long long l, long long h)
{
    now = head;
    for(int k = 0; k < i; k++)
    {
        now = now -> next;
    }
    return now -> array -> range(now -> array -> root, l, h);
}

long long pred()
{
    if(now_tree == NULL)
    {
        return -1;
    }
    else
    {
        BSTnode * now_node = now_tree -> array -> find(now_val);
        if(now_node == now_tree -> array -> smallest())
        {
            now_tree = NULL;
            return -1;
        }
        else
        {
            BSTnode * pred = now_tree -> array -> pred(now_node);
            now_val = pred -> val;
            return now_val;
        }
    }
    return -1;
}

long long succ()
{
    if(now_tree == NULL)
    {
        return -1;
    }
    else
    {
        BSTnode * now_node = now_tree -> array -> find(now_val);
        if(now_node == now_tree -> array -> largest())
        {
            now_tree = NULL;
            return -1;
        }
        else
        {
            BSTnode * succ = now_tree -> array -> succ(now_node);
            now_val = succ -> val;
            return now_val;
        }
    }
    return -1;
}
void init()
{
    head = new node;
}

#endif