#include<bits/stdc++.h>
#include <bitset>
#include <chrono>
#include<unordered_map>
//#include <fstream.h>
using namespace std;
using namespace std::chrono;
unordered_map<char,string> encode;
string tree="";
string temptree=""; //all file str binary
queue<char>t;
FILE*fout;
FILE*fin;
ofstream compressed;
struct node
{
    node *left;
    node *right;
    char x;
    int freq;
    node(char x, int freq)
    {
        left = right = NULL;
        this->x = x;
        this->freq = freq;
    }
};
int get_file_size(std::string filename)
{
    FILE *p_file = NULL;
    p_file = fopen(filename.c_str(),"rb");
    fseek(p_file,0,SEEK_END);
    int sizee = ftell(p_file);
    fclose(p_file);
    return sizee;
}
struct compare
{    bool operator()(node *n1, node *n2)
     {
        if(n1->freq == n2->freq)
            return n1->x < n2->x;
        else
            return n1->freq > n2->freq;
     }
};
int convertBinaryToDecimal(string x)
{
    int decimalNumber = 0, i = 0;
    for(int j=7;j>=0;j--)
    {
        char temp=x.at(j);
        int n=(int)temp-48;

        decimalNumber += n*pow(2,i);
        ++i;
    }
    return decimalNumber;
}
int ccc=0;
node* decodetree()//rebuilding the tree from tree line
{     ccc++;
    char x=t.front();
    if(x=='1')
    {
        t.pop();
        char y=t.front();
        t.pop();
        struct node* n=new node(y,NULL);
        return n;
    }
    else
    {
        t.pop();
        node *left=decodetree();
        node *right=decodetree();
        struct node* n=new node('0',NULL);//creation of internal node
        n->left=left;
        n->right=right;
        return n;
    }
}
void writeinfile()//writing to compressed.txt
{    string filename("input.txt");//test.txt is still open by freopen to read from using scanf//same for example.txt using printf
    FILE* input_file = fopen(filename.c_str(), "r");//reading using cin<<
    string res="";
    unordered_map<char, string>::iterator it;
    int size1=get_file_size(filename);
    int ss=size1;
    char character;
    while (!feof(input_file)&& ss>0)
    {
        character = getc(input_file);
        it=encode.find(character);
        res+=it->second;//placing codes of each char i read from input into res string
        ss--;
    }
    int padding=0;
    if((res.size()%8)!=0)
        padding=8-(res.size()%8);
    fclose(input_file);//only reading from test.txt using scanf now
    int j=padding;
    while(j!=0)
    {
        res+='0';
        j--;
    }
  int i=0;
    string x="";
    char c=padding+'0';
    int out;
    compressed.write((char *)&c,sizeof(char));//printf("%d",padding);
    char n=17;
    compressed.write((char *)&n,sizeof(n));//printf("%c",n);
    while(i<res.size())
    {
        x=res.substr(i,8);
        i+=8;
        out=convertBinaryToDecimal(x);
        char temp=out;
        compressed.write((char *)&temp,sizeof(temp));//printf("%c",temp);//priting char equivilant of each encoded 8 bits
    }
    compressed.write((char *)&n,sizeof(char));//printf("%c",n);//printing the delimiter 17 in the end of data line
}
void decToBinary(int n)
{   int i = 0;
    string temp="";
    while (n > 0) {
        int y=(n%2);
        char x=y+'0';
        temp+=x;//filling temp binary char value of 8 bits of data
        n = n / 2;
    }
    if(temp.size()<8)
    {
        int var=8-temp.size();
        while(var>0)
        {
            var--;
            temptree+='0';
        }
    }
    for(int i=temp.size()-1;i>=0;i--)
       {
           temptree+=temp.at(i);
       }
}
void Encodenode(struct node* root,string str)//at the start str empty//read from input.txt write to compressed.txt
{
     if(!root)
        return;
    else if((root->left==NULL) && (root->right==NULL))//leaf node
      {
            tree+='1';//tree is string tree line in compressed file
            tree+=root->x;
            char c='1';
            compressed.write((char *)&c,sizeof(char));//printf("%c",c);
            compressed.write((char *)&root->x,sizeof(char));//printf("%c",root->x);
            encode[root->x]=str;//encode is a map
            str="";//str represents code/path
      }
      else
      {
            char c='0';
            compressed.write((char *)&c,sizeof(char));//printf("%c",c);
            tree+='0';
            Encodenode(root->left,str+"0");
            Encodenode(root->right,str+"1");
      }
}
void decompress()
{
    ifstream comp("compressed.txt", ios::in | ios::binary);//freopen("example.txt","r",stdin);
    int size1=get_file_size("compressed.txt");
    int ss=size1;
    int padding;
    char character;
    while (1)//scanning tree line
    {
        ss--;
        comp.read((char *)&character,sizeof(character));//scanf("%c",&character);
        if(character==17)
            break;
        t.push(character); //t is a queue representing tree line
    }
    while(1)
    {
        ss--;
        comp.read((char *)&character,sizeof(character));//scanf("%c",&character);
        if(character==17)
            break;
        padding=(int)character-48;
    }
    unsigned char temp;
    while(!comp.eof() && ss>1)
    {
        ss--;
        comp.read((char *)&temp,sizeof(character));//scanf("%c",&temp);
        int x=(int)temp;//from 8 bit char to decimal of data line
        decToBinary(x);
    }
    comp.close();//fclose(stdin);
    freopen("CON","w",stdout);
    struct node*l;
    l= decodetree();//l is the root of the rebuilt tree
    Encodenode(l,"");
    map<char, string>:: iterator z;
    //Remove Padding
    int en=temptree.size();
    string line="";
    while(padding>0)
    {
        temptree.erase(en);
        padding--;
        en--;
    }
    //DECODING
    int counter=0;
    char current;
    string code="";
    string result="";
    struct node*store=l;
    while(counter<temptree.size())
    {
        current=temptree[counter];
        if(current=='1')
        {
            code+=current;
            store=store->right;
        }
        else
        {
            code+=current;
            store=store->left;
        }
        if((store->left==NULL) && (store->right==NULL))
        {  //leaf
            result+=store->x;
            store=l;
        }
        counter++;
    }
    FILE*outptr;
    outptr=freopen("decompressed.txt","w",stdout);
    int len=0;
    while(len<result.length()-1)
    {
        printf("%c",result[len]);
        len++;
    }
    fclose(outptr);
    freopen("CON","w",stdout);
}
void compress()
{
    struct node *root;
    string filename("input.txt");
    vector<char>tempo;
    unordered_map<char,int> characters;
    FILE* input_file = fopen(filename.c_str(), "r");//ifstream myFile (filename, ios::in | ios::binary);
    int size1=get_file_size(filename);
    int ss=size1;
    unsigned char character = 0;
    while (!feof(input_file) && ss>0 )
    {
        character = getc(input_file);//myFile.read ((char *)character, sizeof(character));
        characters[character]++;
        ss--;
    }
    fclose(input_file);//myFile.close();
    priority_queue <node*, vector<node*>, compare > pq;
    unordered_map<char, int>:: iterator p;
    for (p = characters.begin(); p != characters.end(); p++)
    {
        struct node* n=new node(p->first,p->second);
        pq.push(n);
    }
    while (pq.size() != 1)
    {
        struct node *left;
        left= pq.top();
        pq.pop();
        //printf("%d,",left->freq);
        struct node* right;
        right=  pq.top();
        pq.pop();
        //printf("%d,",right->freq);
        struct node* top=new node(NULL, left->freq + right->freq);
        top->left=left;
        top->right=right;
        pq.push(top);
    }
    compressed.open("compressed.txt", ios::out | ios::binary);//freopen("example.txt","w",stdout);
    freopen("input.txt","r",stdin);//freopen("test.txt","r",stdin);
    Encodenode(pq.top(),"");//pq is the huffman tree
    char x=17;
    compressed.write((char *)&x,sizeof(char));//printf("Delimiter=%c\n",x);
    writeinfile();
    compressed.close();

}
int main()
{
    int method;
    cout<<"Please choose the method  1-Compression  2- Decompression \n";
    cin >> method;
    if( method ==  1)
    {
        clock_t start,end;
        start = clock();
        compress(); //strat procedure ine
        end = clock();
        double time1 = (double)(end - start) / CLOCKS_PER_SEC;
        cout << "Time taken by function: " << time1 << " seconds" << endl;
        float size1=get_file_size("input.txt");
        float size2=get_file_size("compressed.txt");
        float CompressionRatio=(float)(size2/size1*1.0) *100;
        cout<< "\n Compression Ratio= "<< CompressionRatio << "%" << endl ;
    }
    else
    {
        clock_t start,end;
        start = clock();
        decompress(); //strat procedure ine
        end = clock();
        double time1 = (double)(end - start) / CLOCKS_PER_SEC;
        cout << "Time taken by function: " << time1 << " seconds" << endl;
    }
    cout<<" Byte "  << '\t' << " Code "<< '\t' << '\t' <<"  New Code " << endl;
    unordered_map<char,string>::iterator mp;
    for (mp = encode.begin(); mp != encode.end(); mp++)
    {
        bitset<8> a(mp->first);
        string x=a.to_string();
        printf(" %d  ",mp->first);
        cout << '\t' ;
        cout << x ;
        cout << '\t' ;
        cout << mp->second << endl;
    }
    return 0;
}
