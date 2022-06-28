#include<iostream>
using namespace std;

const int m = 5;
const int MAX_KEY = m-1;  //每个节点能拥有的最大键数
const int MIN_KEY = (m%2==0)? m/2 - 1 : m/2;   // 每个节点最少拥有的键数 (m/2向上取整+1)
typedef int KeyType;

typedef struct BTNode{
    int keynum;   //节点中关键字个数，即节点大小
    struct BTNode * parent;  //指向双亲节点的指针
    KeyType key[m+1];
    struct BTNode * ptr[m+1];  //子树指针向量
}BTNode, *B_Tree;     //B树节点和其指针类型

//用于B树查找算法的返回值
typedef struct{
    BTNode *pt;  //指向找到的结点
    int i;       //在结点中的关键字序号
    int tag;     //1:查找成功  2:查找失败
}Result, *Ptr_Result;

int Search(B_Tree p, KeyType K);   //B树的查找算法
Ptr_Result SearchBTree(B_Tree T, KeyType K);

B_Tree split(B_Tree p, KeyType &Mid_Key);   //当待插入结点元素个数等于m-1，引起节点分裂
B_Tree Insert(B_Tree T, KeyType K);
B_Tree Insert_up(B_Tree T, KeyType K, B_Tree son_left=nullptr, B_Tree son_right=nullptr);
B_Tree Delete(B_Tree T, KeyType k);
void Direct_Del(B_Tree &node, int pos);  //直接删除键,pos为该键在结点node中的下标
void Direct_Add(B_Tree &node, KeyType key, int flag); //flag=0插在最左边，flag=1插在最右边

bool Borrow_From_LeftBro(B_Tree &node, KeyType &get);//尝试从左紧邻兄弟借点,如果失败返回false,如过成功则删除左兄借出的节点返回true
//注意，如果紧邻左兄能借，则借的一定是左兄的最大键，即中序前置
//左兄借出的值不能直接给借者，因为这样会影响左兄<父母<借者的顺序，借出的值需要替换父母对应键值，再把父母替走的键值给借者
bool Borrow_From_RightBro(B_Tree &node, KeyType &get);//尝试从右紧邻兄弟借点,如果失败返回false,如过成功则删除右兄借出的节点返回true
//注意，如果紧邻右兄能借，则借的一定是右兄的最小键，即中序后置
//右兄借出的值不能直接给借者，因为这样会影响借者<父母<由兄的顺序，借出的值需要替换父母对应键值，再把父母替走的键值给借者
void Merge_Two_Sons(B_Tree &left, B_Tree right);    //合并左右子到左子
B_Tree Merge_With_Sibling(B_Tree T, B_Tree bottom);   //从底向上递归合并同级
B_Tree Merge_Left_Father_Right(B_Tree Right, int &pos_father);       //融合左子，父键，右子

int Search(B_Tree p, KeyType K)
{
    //定位到合适区间
    int i;
    for(i=0; i<p->keynum && K>=p->key[i+1]; i++);
    return i;
    // p->key[i] <= K < p->key[i+1]
}

Ptr_Result SearchBTree(B_Tree T, KeyType K)
{
    B_Tree p,q;
    bool found; 
    int i,j;
    Ptr_Result R = new Result;
    //初始化，p指向待查结点，q指向p的双亲
    p=T; q=nullptr;
    found=false;    i=j=0;
    while(p && !found){   //若p为空指针(最终未找到)或found=true(找到)会终止循环
        i = Search(p,K);  //在p->key[1..keynum]中查找i的区间
        //结果是p->key[i] <= K < p->key[i+1]
        if(i>0 && p->key[i]==K){
            found = true;   //找到待查关键字
        }
        else{
            q=p;  //q指向p的双亲
            p=p->ptr[i];
        }
    }
    if(found){  //查找成功：p的pt域所指结点中的第i个关键字等于k
        R->pt=p;
        R->i=i;
        R->tag=1;  //查找成功
    }
    else{       //查找失败，但查找的结果对插入操作有用，它说明
        //关键字K应该插入在pt所指结点的第i个和第i+1个关键字之间，此时这之间的pt域为空nullptr
        //此时p为空指针，q为待插入位置的指针
        R->pt=q;
        R->i=i;
        R->tag=0;  //查找失败
    }

    return R;
}

B_Tree split(B_Tree p, KeyType &Mid_Key)
{
    //结点p中包括m个关键字，从中分裂出一个新的结点
    B_Tree q;
    int k,mid,j;
    q = new BTNode;
    mid = (m+1)/2;

    q->ptr[0]=p->ptr[mid];
    Mid_Key = p->key[mid];
    for(j=1,k=mid+1;k<=m;k++){
        q->key[j]=p->key[k];
        q->ptr[j]=p->ptr[k];
        j++;
    } //将p的后半部分转移到新节点q中
    q->keynum=m-mid;
    p->keynum=mid-1;
    return q;
}

B_Tree Insert(B_Tree T, KeyType K)
{
    if(T==nullptr){  //新的根节点
        T = new BTNode;
        T->keynum=1;
        T->key[1]=K;
        T->parent=nullptr;
        T->ptr[0]=nullptr;
        T->ptr[1]=nullptr;
        return T;
    }
    Ptr_Result node=SearchBTree(T,K);
    if(node->tag==1){
        cout << "This node has existed, cancel the insert";
        return T;
    }
    node->pt->keynum++;
    //插入关键点K,K要插入在关键点i和i+1之间，则占用原i+1的位置，后面的顺延一位
    for(int j=node->pt->keynum; j>(node->i+1); j--){
        node->pt->key[j]=node->pt->key[j-1];
        node->pt->ptr[j]=node->pt->ptr[j-1];
    }
    node->pt->key[node->i+1]=K;
    node->pt->ptr[node->i+1]=nullptr;

    if(node->pt->keynum==m){
        KeyType Mid_Key;  //分割后剩余的中间节点，要插入到其父节点中
        B_Tree right=split(node->pt,Mid_Key);
        //左边部分为原node->pt
        right->parent=node->pt->parent;  //同一个父节点
        T = Insert_up(node->pt->parent,Mid_Key,node->pt,right);
        if(node->pt->parent==nullptr){
            right->parent=node->pt->parent=T;   //为了适应初始时只有一层的情况，此时node->parent为nullptr，所以需要构造新根后再令左子和右子指向新根
        }
    }
    return T;
}

B_Tree Insert_up(B_Tree T, KeyType K, B_Tree son_left, B_Tree son_right)
{
    if(T==nullptr){  //新的根节点
        T = new BTNode;
        T->keynum=1;
        T->key[1]=K;
        T->parent=nullptr;
        T->ptr[0]=son_left;
        T->ptr[1]=son_right;
        return T;
    }
    int i=Search(T,K);
    T->keynum++;
    //插入关键点K,K要插入在关键点i和i+1之间，则占用原i+1的位置，后面的顺延一位
    for(int j=T->keynum; j>i+1; j--){
        T->key[j]=T->key[j-1];
        T->ptr[j]=T->ptr[j-1];
    }
    T->key[i+1]=K;
    T->ptr[i+1]=son_right;

    if(T->keynum==m){
        KeyType Mid_Key;  //分割后剩余的中间节点，要插入到其父节点中
        B_Tree right=split(T,Mid_Key);
        //左边部分为原node->pt
        right->parent=T->parent;  //同一个父节点
        T = Insert_up(T->parent,Mid_Key,T,right);
    }
    return T;
}

B_Tree Delete(B_Tree T, KeyType k)
{
    Ptr_Result del = SearchBTree(T,k);
    if(del->tag==2){
        cout << "Key " << k << "not exist, cancel the delete";
        return T;
    }
    
    if(del->pt->ptr[0]==nullptr){    //情况一：待删除的键位于叶中，依据是该节点的ptr[0]为空指针，如果非叶子节点ptr[0]一定不为空
        //当删除的键位于叶中时，分2种情况(其中有个特殊情况是del本身就是根)
        if(del->pt->keynum>MIN_KEY || del->pt->parent==nullptr){    //情况1：删除该键不会违反节点应持有最小键数的属性或本身是根节点，没有减的要求
            Direct_Del(del->pt,del->i);
        }
        else{                           //情况2：删除该键会使节点持有键数小于最少键数，则需要从左向右顺序先尝试问紧邻兄弟节点借用一个键
            KeyType borrow;
            if(Borrow_From_LeftBro(del->pt,borrow)){ 
                //如果从左兄成功借到，且将父母节点对应键值存入了borrow，现在将borrow插入del->pt节点
                //注意因为这里的大前提是del->pt->keynum<=MIN_KEY，所以不存在插入后超出键数限制的情况
                //左边借来的插入最左边
                Direct_Add(del->pt,borrow,0);
            }
            else if(Borrow_From_RightBro(del->pt,borrow)){
                //如果从右兄成功借到，且将父母节点对应键值存入了borrow，现在将borrow插入del->pt节点
                //注意因为这里的大前提是del->pt->keynum<=MIN_KEY，所以不存在插入后超出键数限制的情况
                //右边借来的插入最右边
                Direct_Add(del->pt,borrow,1);
            }
            else{
                //两边都无兄弟或右兄弟但不满足键数大于最小限制
                //先删除待删除点，然后调用合并同级函数
                for(int i=del->i;i<del->pt->keynum;i++){
                    //del->pr[del->i-1]仍指向left_son,不发生改变
                    del->pt->key[i] = del->pt->key[i+1];
                    del->pt->ptr[i] = del->pt->ptr[i+1];
                }
                del->pt->ptr[del->pt->keynum] = nullptr;  //该步可以省略
                del->pt->keynum--;
                T = Merge_With_Sibling(T,del->pt);
            }
        }
    }
    else{       //情况二：待删除的键位于内部节点中
        //当删除的键位于内部节点中时，同样分两种情况
        //情况1是待删除键的左子或右子丰满——即能给待删除键替换
        B_Tree left_son = del->pt->ptr[del->i];
        B_Tree right_son = del->pt->ptr[del->i+1];
        if(left_son->keynum>MIN_KEY){   //先尝试问左子借
            //左子能借则借中序前置，即左子最大键值
            del->pt->key[del->i] = left_son->key[left_son->keynum];
            //因为左子节点不一定是叶子节点，所以不能用Direct_del，应递归调用Delete删除左子借出的节点
            T = Delete(T,left_son->key[left_son->keynum]);
        }
        else if(right_son->keynum>MIN_KEY){     //再尝试问右子借
            //右子能借则借中序后置，即右子最小键值
            del->pt->key[del->i] = right_son->key[1];
            //因为右子节点不一定是叶子节点，所以不能用Direct_del，应递归调用Delete删除右子借出的节点
            T = Delete(T,right_son->key[1]);
        }
        else{      //左右子都借不了
            //此时应合并左子与右子，并删除待删除节点
            //先合并左右子，合并到左子上去（因为大前提是左右子都不丰满，所以合并后也不会超出最大键数
            //注意到因为左右子都不丰满，而左右子都满足最小键要求，说明左右子都恰好有MIN_KEY个键数
            //该合并是一个向下递归的过程，递归的终点是叶子节点
            Merge_Two_Sons(left_son,right_son);
            //然后删除待删除节点
            for(int i=del->i;i<del->pt->keynum;i++){
                //del->ptr[del->i-1]仍指向left_son,不发生改变
                del->pt->key[i] = del->pt->key[i+1];
                del->pt->ptr[i] = del->pt->ptr[i+1];
            }
            del->pt->ptr[del->pt->keynum] = nullptr;  //该步可以省略
            del->pt->keynum--;

            if(del->pt->keynum==0 && del->pt->parent==nullptr){
                //特殊情况，即删除的是根节点且删除后根没了，则直接返回left_son为新根即可
                T = left_son;
            }

            //删除后有可能删除后的节点键数小于最小键数，则跟情况一的情况2的第三中分支情况一样，需要同级合并
            if(del->pt->keynum<MIN_KEY){
                T = Merge_With_Sibling(T,del->pt);
            }
        }
    }

    return T;
}

void Direct_Del(B_Tree &node, int pos)
{
    //删除结点node下标为node的键
    for(int i=pos; i<node->keynum; i++){
        node->key[i] = node->key[i+1];
        //因为叶子节点指针都为空，所以指针不用移动
    }
    node->keynum--;   //键数-1
}

void Direct_Add(B_Tree &node, KeyType key, int flag)
{
    if(flag==0){
        //插在最左边
        //与删除一样，因为是叶子节点，指针都为空，不用移动指针，只移动键值
        for(int i=1;i<=node->keynum;i++){
            node->key[i+1] = node->key[i];
        }
        node->key[1] = key;
        node->keynum++;
    }
    else{
        //插在最右边
        node->key[node->keynum+1]=key;
        node->keynum++;
    }
}

bool Borrow_From_LeftBro(B_Tree &node, KeyType &get)
{
    B_Tree parent = node->parent;
    int pos=-1;
    for(int i=0;i<=parent->keynum;i++){
        if(parent->ptr[i]==node){
            pos = i;
            break;
        }
    }
    if(pos==-1){
        cout << "Warning: wrong ptr!" << endl;
    }
    else if(pos==0){
        //意味着node无紧邻的左兄弟，返回false
        return false;
    }
    else{
        B_Tree Left_Bro = parent->ptr[pos-1];
        if(Left_Bro->keynum>MIN_KEY){
            //注意，如果紧邻左兄能借，则借的一定是左兄的最大键，即中序前置
            get = parent->key[pos];     //借者得到的是父母节点位于借者和左兄之间的key值
            parent->key[pos] = Left_Bro->key[Left_Bro->keynum];     //父母节点位于借者和左兄之间的key值被左兄借出的键值替换
            //既然借了，那把左兄借出的键删了
            //又因为叶子的兄弟必也是叶子，所以调用Direct_Del函数即可
            Direct_Del(Left_Bro,Left_Bro->keynum);
            return true;
        }
        else{
            //左兄弟键不大于最少键，没法借，返回false
            return false;
        }
    }
    return true;
}

bool Borrow_From_RightBro(B_Tree &node, KeyType &get)
{
    B_Tree parent = node->parent;
    int pos=-1;
    for(int i=0;i<=parent->keynum;i++){
        if(parent->ptr[i]==node){
            pos = i;
            break;
        }
    }
    if(pos==-1){
        cout << "Warning: wrong ptr!" << endl;
    }
    else if(pos==parent->keynum){
        //意味着node无紧邻的右兄弟，返回false
        return false;
    }
    else{
        B_Tree Right_Bro = parent->ptr[pos+1];
        if(Right_Bro->keynum>MIN_KEY){
            //注意，如果紧邻右兄能借，则借的一定是右兄的最小键，即中序后置
            get = parent->key[pos];     //借者得到的是父母节点位于借者和右兄之间的key值
            parent->key[pos] = Right_Bro->key[1];       //父母节点位于借者和右兄之间的key值被右兄借出的键值替换
            //既然借了，那把右兄借出的键删了
            //又因为叶子的兄弟必也是叶子，所以调用Direct_Del函数即可
            Direct_Del(Right_Bro,1);
            return true;
        }
        else{
            //右兄弟键不大于最少键，没法借，返回false
            return false;
        }
    }
    return true;
}

void Merge_Two_Sons(B_Tree &left, B_Tree right)
{
    if(left->ptr[left->keynum]!=nullptr){
        Merge_Two_Sons(left->ptr[left->keynum],right->ptr[0]);
        //向下递归，融合左节点最右子和右结点最左子
        //递归的终点是左节点无子即左节点就是叶子节点
    }
    for(int i=left->keynum+1;i<=left->keynum+right->keynum;i++){
        left->key[i] = right->key[i-left->keynum];
        left->ptr[i] = right->ptr[i-left->keynum];
    }
    left->keynum += right->keynum;
}

B_Tree Merge_With_Sibling(B_Tree T, B_Tree bottom)
{
    //递归终止条件——根节点，它无最小键数要求，也没有同级，则无需再向上递归合并
    if(bottom->parent==nullptr){
        return bottom;
    }
    else{
        //先尝试问同级借键，这里操作与Delete的情况一类似
        KeyType borrow;
        if(Borrow_From_LeftBro(bottom,borrow)){ 
            //如果从左兄成功借到，且将父母节点对应键值存入了borrow，现在将borrow插入del->pt节点
            //注意因为这里的大前提是del->pt->keynum<=MIN_KEY，所以不存在插入后超出键数限制的情况
            //左边借来的插入最左边
            Direct_Add(bottom,borrow,0);
            return T;
        }
        else if(Borrow_From_RightBro(bottom,borrow)){
            //如果从右兄成功借到，且将父母节点对应键值存入了borrow，现在将borrow插入del->pt节点
            //注意因为这里的大前提是del->pt->keynum<=MIN_KEY，所以不存在插入后超出键数限制的情况
            //右边借来的插入最右边
            Direct_Add(bottom,borrow,1);
            return T;
        }
        else{
            //两边都无兄弟或右兄弟但不满足键数大于最小限制
            //此时需要合并bottom与其父节点与其左兄弟(除非其无左兄弟时则用右兄弟，但不存在没兄弟的情况)
            int pos_father;
            B_Tree parent = bottom->parent;
            B_Tree Merged_Son = Merge_Left_Father_Right(bottom, pos_father);

            //有特殊情况就是parent为根且parent删除完后没键了，则Merge_Son直接为新根，返回Merge_Son即可
            if(parent->parent==nullptr && parent->keynum-1==0){
                return Merged_Son;
            }

            //然后删除待删除节点
            parent->ptr[pos_father-1] = Merged_Son;
            for(int i=pos_father;i<parent->keynum;i++){
                parent->key[i] = parent->key[i+1];
                parent->ptr[i] = parent->ptr[i+1];
            }
            parent->ptr[parent->keynum] = nullptr;  //该步可以省略
            parent->keynum--;

            //递归调用Merge_With_Sibling()
            return Merge_With_Sibling(T,parent);
        }
    }
    return T;
}

B_Tree Merge_Left_Father_Right(B_Tree Right, int &pos_father)
{
    B_Tree parent = Right->parent;
    int pos=-1;
    for(int i=0;i<=parent->keynum;i++){
        if(parent->ptr[i]==Right){
            pos = i;
            break;
        }
    }
    pos_father = pos;
    if(pos==0){
        //说明无左兄弟，则反之融合右兄弟
        B_Tree Right_right = parent->ptr[pos+1];
        Right->key[Right->keynum+1] = parent->key[pos];
        Right->ptr[Right->keynum+1] = Right_right->ptr[0];
        for(int i=Right->keynum+2;i<=Right->keynum+1+Right_right->keynum;i++){
            Right->key[i] = Right_right->key[i-Right->keynum-1];
            Right->ptr[i] = Right_right->ptr[i-Right->keynum-1];
        }
        Right->keynum = Right->keynum + 1 + Right_right->keynum;
        return Right;
    }
    else{
        //有左兄弟，融合左兄弟
        B_Tree Left = parent->ptr[pos-1];
        Left->key[Left->keynum+1] = parent->key[pos];
        Left->ptr[Left->keynum+1] = Right->ptr[0];
        for(int i=Left->keynum+2;i<=Left->keynum+1+Right->keynum;i++){
            Left->key[i] = Right->key[i-Left->keynum-1];
            Left->ptr[i] = Right->ptr[i-Left->keynum-1];
        }
        Left->keynum = Left->keynum + 1 + Right->keynum;
        return Left;
    }
}

int main(){

    //见ppt 66/98图

    B_Tree T=nullptr;
    T=Insert(T,1);
    cout << "insert 1" << endl;
    T=Insert(T,3);
    cout << "insert 3" << endl;
    cout << T->key[1] << " " << T->key[2] << endl;
    T=Insert(T,7);
    cout << "insert 7" << endl;
    cout << T->key[1] << " " << T->key[2] << " " << T->key[3] << endl;
    T=Insert(T,14);
    cout << "insert 14" << endl;
    cout << T->key[1] << " " << T->key[2] << " " << T->key[3] << " " << T->key[4] << endl;
    T=Insert(T,8);
    cout << "insert 8" << endl;
    cout << T->key[1] << endl;
    cout << T->ptr[0]->key[1] << endl;
    cout << T->ptr[0]->key[2] << endl;
    cout << T->ptr[1]->key[1] << endl;
    cout << T->ptr[1]->key[2] << endl;

    T=Insert(T,5);
    cout << "insert 5" << endl;
    T=Insert(T,11);
    cout << "insert 11" << endl;
    T=Insert(T,17);
    cout << "insert 17" << endl;
    T=Insert(T,13);
    cout << "insert 13" << endl;

    cout << "1 floor: " << T->key[1] << " " << T->key[2] << endl;
    cout << "2 floor: " << T->ptr[0]->key[1] << " " << T->ptr[0]->key[2] << " " << T->ptr[0]->key[3] << " " << T->ptr[1]->key[1] << " " << T->ptr[1]->key[2] << " " << T->ptr[2]->key[1] << " " << T->ptr[2]->key[2] << endl;

    T=Insert(T,6);
    cout << "insert 6" << endl;
    T=Insert(T,12);
    cout << "insert 12" << endl;
    T=Insert(T,20);
    cout << "insert 20" << endl;
    T=Insert(T,23);
    cout << "insert 23" << endl;
    T=Insert(T,26);
    cout << "insert 26" << endl;
    T=Insert(T,4);
    cout << "insert 4" << endl;
    T=Insert(T,16);
    cout << "insert 16" << endl;
    T=Insert(T,18);
    cout << "insert 18" << endl;
    T=Insert(T,24);
    cout << "insert 24" << endl;
    T=Insert(T,25);
    cout << "insert 25" << endl;
    cout << T->key[1] << T->key[2] << T->key[3] <<T->key[4] << endl;
    cout << T->ptr[0]->keynum << T->ptr[1]->keynum << T->ptr[2]->keynum << T->ptr[3]->keynum << T->ptr[4]->keynum << endl;
    T=Insert(T,19);
    cout << "insert 19" << endl;

    cout << "Result is: " << endl;
    cout << "1 floor: " << T->key[1] << endl;
    cout << "2 floor: " << T->ptr[0]->key[1] << " " << T->ptr[0]->key[2] << " " << T->ptr[1]->key[1] << " " << T->ptr[1]->key[2] << endl;
    cout << "3 floor: " << T->ptr[0]->ptr[0]->key[1] << " " << T->ptr[0]->ptr[0]->key[2] << " " << T->ptr[0]->ptr[1]->key[1] << " " << T->ptr[0]->ptr[1]->key[2] << " "
                        << T->ptr[0]->ptr[2]->key[1] << " " << T->ptr[0]->ptr[2]->key[2] << " " << T->ptr[0]->ptr[2]->key[3] << " "
                        << T->ptr[1]->ptr[0]->key[1] << " " << T->ptr[1]->ptr[0]->key[2] << " " << T->ptr[1]->ptr[1]->key[1] << " " << T->ptr[1]->ptr[1]->key[2] << " "
                        << T->ptr[1]->ptr[2]->key[1] << " " << T->ptr[1]->ptr[2]->key[2] << " " << T->ptr[1]->ptr[2]->key[3] << " " << T->ptr[1]->ptr[2]->key[4] << endl;

    return 0;
}