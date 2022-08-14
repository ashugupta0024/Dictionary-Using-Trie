#include<iostream>
#include<string.h>
#include<algorithm>
#include<stdlib.h>
#include<vector>
#include<fstream>
#include<unistd.h>
#include<conio.h>
#define ALPHABETS 26
using namespace std;

struct TrieNode{
    TrieNode(){
        meaning = "";
        for(int i=0;i<ALPHABETS;i++)  //default constructor
				child[i]=NULL;
    }
    string meaning;
    TrieNode *child[ALPHABETS];
};
class Trie
{
public:
    Trie();
    ~Trie();
    void insertWord(string s,string meaning);
    void findWord(string,bool&);
    void searchWord(string);
    bool autoCompleteWord(TrieNode *root,string s,vector<string>&);
    bool isEmptyWord(TrieNode*);
    TrieNode*removeUntil(TrieNode*,string,int);
    void remove(string);


private:
    TrieNode *root;
};
Trie::Trie()
{
    root = new TrieNode();
}
Trie::~Trie()
{
    // Free memory
}
void Trie::insertWord(string word,string meaning) //insert word and meaning into trie
{
        int i,idx;
        TrieNode *curr=root;
        for(i=0;i<word.length();i++)
        {
            idx=word[i]-'a';
            if(curr->child[idx]==NULL)
                curr->child[idx]=new TrieNode();
            curr=curr->child[idx];
        }
        curr->meaning=meaning;
}
void Trie::findWord(string word,bool& bb) //find word in trie
{
    TrieNode *curr = root;
    int i,idx;
    for(i =0;i<word.size();i++)
    {
        idx = word[i]-'a';
        if(curr->child[idx] == NULL)
        {
                bb = false;
                return;
        }
         curr = curr->child[idx];
    }
    if(curr->meaning == "")
    {
        bb = false;
        return;
    }
    if(curr->meaning != "")
    {
        bb = true;
        return;
    }
}
bool Trie::autoCompleteWord(TrieNode *temp,string word, vector<string> &similarWords) //autocomplete feature
{
    TrieNode *curr=temp;
	if(temp->meaning!="")
		{
			similarWords.push_back(word);
		}
	for(int i=0;i<ALPHABETS;i++)
	{
			if(curr->child[i])
				autoCompleteWord(curr->child[i],word+(char)(i+'a'),similarWords);
	}
}
void Trie::searchWord(string word) // search word in a dictionary
{
    int i,idx;
	TrieNode* curr=root;
	vector <string> similarWords;
	for(i=0;i<word.length();i++)
	{
		idx=word[i]-'a';
		if(curr->child[idx]==NULL)
			{
				autoCompleteWord(curr,word.substr(0,i),similarWords);
				break;
			}
		curr=curr->child[idx];
	}
	if(curr->meaning=="")
		autoCompleteWord(curr,word,similarWords);

	if(curr->meaning!="")
	cout<<"\nThe word \""<<word<<"\" was found with meaning \""<<curr->meaning<<"\"";
	else
	{
		cout<<"\nThe word \""<<word<<"\" was not found";
		if(similarWords.size()==0)
			return;
        cout << "\n\tAutoComplete feature :";
		cout<<"\n\tDid you mean? ";
		for(int i =0;i<similarWords.size();i++){
            cout << "\n\t" <<similarWords[i];
		}

	}
}
bool Trie:: isEmptyWord(TrieNode* curr)
{
	int i;
	for(i=0;i<ALPHABETS;i++)
		if(curr->child[i])
			return 0;
	return 1;
}
TrieNode* Trie::removeUntil(TrieNode*curr,string word,int depth=0) //remove words from trie
{
	if(depth==word.size())
	{
		curr->meaning="";
		if(isEmptyWord(curr))
			{
				delete(curr);
				curr=NULL;
			}
		return curr;

	}
	int idx=word[depth]-'a';
	curr->child[idx]=removeUntil(curr->child[idx],word, depth+1);
	if(isEmptyWord(curr) && curr->meaning=="")
	{
		delete(curr);
		curr=NULL;
	}
	return curr;
}

void Trie::remove(string word)
{
	removeUntil(root,word);
	cout<<"\n\tThe word \""<<word<<" has been successfully removed. ";

}

bool loadDictionary(Trie* trie,string st)
{
    //ofstream
    //This data type represents the output file stream and is used to create files and to write information to files.

    ifstream words; //This data type represents the input file stream and is used to read information from files.
    ifstream input;
    words.open(st.c_str()); //The c_str() method converts a string to an array of characters with a null character at the end. The function takes in no parameters and returns a pointer to this character array.
    if(!words.is_open()) //Returns whether the stream is currently associated to a file.
    {
        cout<<"Could not open Dictionary file"<<endl;
        return false;
    }
    while(!words.eof())
    {
        string s,m;
        words >> s;
        words >> m;
        trie->insertWord(s,m);
    }
    words.close();
    input.close();
    return true;

}
void WriteNewWord(Trie *trie)
{
	cout<<"Enter the word : ";
	string NewWord;
	bool isfind ;
	cin>>NewWord;
    transform(NewWord.begin(), NewWord.end(), NewWord.begin(), ::tolower); //to lower case
	trie->findWord(NewWord,isfind);
        if(isfind)
        {
            cout << "\n\tWord already exists in the dictionary" << endl;
            return;
        }
	cout << "Enter Meaning : ";
	string meaning;
	cin>> meaning;
	cout<<endl<<"Meaning is "<<meaning<<endl;
	transform(meaning.begin(), meaning.end(), meaning.begin(), ::tolower);
	bool OnlyAlpha=true;
	for(int i=0;i<NewWord.length();i++)
	{
		if(!isalpha(NewWord[i]))
			{
				OnlyAlpha=false;
				break;
			}
	}
	if(OnlyAlpha)
	{
        ofstream out;
        out.open("wordlist.txt",ios::app);
        if(!out.is_open())
        {
            cout<<"Sorry!\nCould not open the dictionary!\n";
            out.close();
            return;
			}
        else
        {
                out<<NewWord<< " "<<meaning<<"\n";
                cout<<"\nSuccessfully loaded in the dictionary!\n";
                out.close();
                trie->insertWord(NewWord,meaning);
                return;
        }
    }
	else
	{
		cout<<"\nNot a valid word!\n";
		return;
	}
}

int main()
{
	Trie* trie = new Trie(); //Where new is used to allocate memory for a C++ class object, the object's constructor is called after the memory is allocated
    char mode;
    cout<<"Loading the dictionary file"<<endl;
    loadDictionary(trie,"wordlist.txt");
    while(1)
    {
        cout<<endl<<endl;
        cout<<"Interactive mode,press "<<endl;
        cout<<"1: Search"<<endl;
        cout<<"2: Enter new words into the dictionary\n";
        cout<<"3: Remove Word"<<endl;
        cout<<"4: Quit"<<endl<<endl;
        //std::cin.ignore(std::numeric_limits<std::streamsize>::max());
        cin>>mode;
        if(isalpha(mode))
        {
        	cout<<"Invalid Input!\n";
        	cout<<"Enter either 1 or 2..";
            continue;
        }
        switch(mode)
        {
        case '1':
        {
            string s;
            cout<<"Enter the partial word : ";
            cin>>s;
            transform(s.begin(), s.end(), s.begin(), ::tolower);
            trie->searchWord(s);
        }
        continue;
        case '2':
        	WriteNewWord(trie);
        	continue;
        case '3':
            {
            cout << "Enter the Word you want to delete :";
            string word;
            cin >> word;
            transform(word.begin(), word.end(), word.begin(), ::tolower);
            trie->remove(word);
            }
            continue;"E:\Programming\Sample Projects\Dictioary-using-Trie-main\Dictioary-using-Trie-main\wordlist.txt";
        case '4':
            delete trie;
            return 0;

        default:
        	cout<<"Invalid input!";
        	cout<<"\nEnter either 1 or 2..";
            continue;
        }
    }
}



