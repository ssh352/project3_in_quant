#include <iostream>
#include <string>
#include <unistd.h>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <fstream>
#include <algorithm>
#include <queue>
#include <stack>
#include <cstdio>

using namespace std;

// structure MagicWord --MagicWord contains the magic word substring,
// the value of the substring and the next array of the substring
struct MagicWord{

	string subStr;
	int value;
	int len;
	vector<int> next;//the next array of the substring used for KMP

	MagicWord(){
		subStr = "";
		value = 0;
		len = 0;
	}

	MagicWord(string w,int v){
		subStr = w;
		value = v;
		len = w.size();
	}

	bool operator < (const MagicWord& right) const {  
	  return value < right.value;  
	}//asscend

	bool operator > (const MagicWord& right) const {  
	  return value > right.value;  
	}//desscend

};

// structure OutWord --OutWord contains the matched magicword substring(the previous removed word)
// and the first index matched in total string
struct OutWord{
	string outWord;
	int outPlace;
	OutWord(){
		outPlace = -1;
		outWord = "none";
	}
	OutWord(string w,int p){
		outWord = w;
		outPlace = p;
	}
};

// structure Node -- the Node is to save some infomation of the tree doing dfs
// It contains the current string (removed some magicwords), the value of the current node
// and the previous removed word and it's index matched in total string
struct Node{
	string nodeStr;
	vector<OutWord> vecOut;
	int nodeValue;
	Node(){
		nodeStr = "";
		nodeValue = 0;
	}
	Node(string s, vector<OutWord> v, int total = 0){
		nodeStr = s;
		vecOut = v;
		nodeValue = total;
	}
	Node(string s, int total = 0){
		nodeStr = s;
		nodeValue = total;
	}
};

// MagicProcess --  maintain the whole process finding the magic word
// including read file, sort, find solution by dfs, print answer
class MagicProcess{
	
	private:
		int n;	// the length of string
		int k;	// the total number of magicwords
		string totalStr;	//the total string to find the magicwords
		vector<MagicWord> vecWord;	// the vector cotains all the magicwords
	
	public:
		
		// Summary: constructed function, initialize some private variable
		MagicProcess(){
			n = 0;
			k = 0;
			totalStr = "";
		}
		
		// Summary: destructor of the class MagicProcess.
		~MagicProcess(){
		}
		
		// Summary: calculate the next[]
		// Parameters:
		//      str:point to the string want to calculate next[].*    
		//      next : the int next array to calculate.
		//		len: the size of the string.
		// Return : none.
		void calNext(const char *str, int *next, int len){
			// initialize next[0]
			next[0] = -1;
			int k = -1;
			for (int q=1;q<=len -1 ;q++){
				while(k > -1 && str[k + 1]!= str[q]){
					k = next[k];
				}
				if(str[k+1]==str[q]){
					k = k+1;
				}
				next[q] = k;
			}
		}
		
		// Summary: match the substring by KMP
		// Parameters:
		//      str:the father string to match. 
		//      subStr : the subStr to match.
		//		next: the next array of the subStr.
		// Return : the first index of the matched substring(-1 if unmatched).
		int KMP(string str,string subStr, vector<int> next){
			int sLen = str.size();
			int subLen = subStr.size();
			int k = -1;
			if(sLen < subLen) return -1;
			for(int i=0;i<sLen;i++){
		
				// unmatched and move to next[k]
				while(k>-1 && subStr[k+1]!=str[i]){
					k = next[k];
				}
				// matched a char and move
				if(subStr [k + 1] == str[i]){
					k = k + 1;
				}
				// matched all
				if(k == subLen-1){
					return i-subLen+1;
				}
			}
			return -1;
		}
		
		// Summary: match the substring, remove it and add the value
		// Parameters:
		//      totalStr:the reference of the father string to match, may change when return. 
		//      mw : the class contain the subStr, next array of the subStr.
		//		totalValue: the reference of the current value.
		// Return : the first index of the matched substring(-1 if unmatched).
		int removeSubStr(string &totalStr,MagicWord mw, int &totalValue){
			//remove the substring
			string tmps = totalStr;
			int start = KMP(tmps,mw.subStr,mw.next);
			//cout<<"KMP = "<<start<<endl;
			if(start != -1){
				totalStr = totalStr.substr(0,start)+totalStr.substr(start+mw.len,totalStr.size());
				totalValue += mw.value;
			}
			return start;
		}
		
		// Summary: find the best solution by dfs
		// Parameters:
		//      root:the root Node contains the initial string and value. 
		//      vecWord : the vector contains all the MagicWord.
		// Return : the Node with the max value.
		Node dfsFindMax(Node root,vector<MagicWord> vecWord){
			//initialize the solution maxNode
		    Node maxNode = root;
		
		    //cout<<"totalStr now = "<<root.str<<endl;
		    //queue<Node> qNode;
		
		    //stack to cotain the nodes when doing dfs
		    stack<Node> stNode;
		
		    //initialize the stack
		    stNode.push(root);
		    while(!stNode.empty()){
		
		    	//current node
		    	Node tmpN;
		    	tmpN = stNode.top();
		    	stNode.pop();
	
		    	//update the solution maxNode
		    	if(tmpN.nodeValue > maxNode.nodeValue){
		    		maxNode = tmpN;
		    	}
		    	
		    	
		    	if(tmpN.nodeStr.empty()) continue;
		
		    	//get all the possible child neighbor node of current node
		    	for(int i = 0;i < vecWord.size();i++){
		
			    	//cout<<"substr["<<i<<"] = "<<vecWord[i].subStr<<" len = "<<vecWord[i].len<<endl;
			    	// for (int j=0;j<tmpWord[i].next.size();j++){
			    	// 	cout<<"next["<<j<<"] = "<<tmpWord[i].next[j]<<endl;
			    	// }
		
			    	//temp variables
			    	string nodeStr = tmpN.nodeStr;
			    	int nodeValue = tmpN.nodeValue;
			    	vector<OutWord> vecOut = tmpN.vecOut;
			    	
			    	if(nodeStr.size()<vecWord[i].subStr.size()){
			    		continue;
					}
//					cout<<"+ the str now = "<<nodeStr<<" value = "<<nodeValue<<endl;
//					cout<<"nodeStr size:"<<nodeStr.size()<<" subStr size:"<<vecWord[i].subStr.size()<<endl;
		
			    	//the string of current node contains the substring
			    	int outPlace = removeSubStr(nodeStr,vecWord[i],nodeValue);
			    	if( outPlace != -1){
			    		//find a new child neighbor node and put it into stack
			    		OutWord output(vecWord[i].subStr,outPlace);
			    		vecOut.push_back(output);
			    		Node nghbr(nodeStr,vecOut,nodeValue);
//						cout<<"- the str now = "<<nghbr.nodeStr<<" value = "<<nghbr.nodeValue<<endl;
//						for(int i = 0; i < nghbr.vecOut.size();i++){
//							cout<<"out = "<<nghbr.vecOut[i].outWord<<" "<<nghbr.vecOut[i].outPlace<<endl;
//						}
			    		stNode.push(nghbr);
			    	}
			    	
			    }
		
			    // while(!stNode.empty()){
			    // 	Node tmpN;
			    // 	tmpN = stNode.top();
			    // 	stNode.pop();
			    // 	cout<<"nodeStr now = "<<tmpN.nodeStr<<" nodeValue now = "<<tmpN.nodeValue<<endl;
			    // }
		    }
		
		    //as it means
		    return maxNode;
		}
		
		// Summary: read the file
		// Parameters:
		//      inFile:the path string of the file. 
		// Return : none.
		void readFileByStr(string inFile){
			if(inFile.empty()){
		    	inFile = "in";
		    }
		
		    ifstream inputFile(inFile.c_str());
		
		
		    inputFile>>n>>k>>totalStr;
		    // cout<<n<<endl;
		    // cout<<k<<endl;
		    // cout<<totalStr<<endl;
		    
		    for(int i=0;i<k;i++){
		    	string tmps;
		    	double tmpd;
		
		    	inputFile>>tmps>>tmpd;
		    	MagicWord mw(tmps,tmpd);
		    	
		    	//calculate the next array
		    	int *nxt = new int [mw.len];
		    	calNext(mw.subStr.c_str(),nxt,mw.subStr.size());
		    	for(int j = 0;j<mw.len;j++){
		    		//cout<<"nxt["<<i<<"] = "<<nxt[i]<<endl;
		    		mw.next.push_back(nxt[j]);
		    	}
		
		    	//push to the vector
		    	vecWord.push_back(mw);
		    }
		}
		
		// Summary: show the route and value of the Node
		// Parameters:
		//      maxNode:the best node. 
		// Return : none.
		void showAnswer(Node maxNode){
			// for(int i = 0; i < maxNode.vecOut.size();i++){
			// 	cout<<"out = "<<maxNode.vecOut[i].outPlace<<" "<<maxNode.vecOut[i].outWord<<endl;
			// }
			// cout<<"current string is: "<<maxNode.nodeStr<<" the value is: "<<maxNode.nodeValue<<endl;
			for(int i = 0; i < maxNode.vecOut.size();i++){
				cout<<maxNode.vecOut[i].outPlace<<" "<<maxNode.vecOut[i].outWord<<endl;
			}
			cout<<maxNode.nodeValue<<endl;
		}
		
		// Summary: sort the word vector by the value of the magic word
		// Parameters:
		//     none. 
		// Return : none.
		void sortVecWordDesc(){
			sort(vecWord.begin(), vecWord.end(),less<MagicWord>());
		}
		
		// Summary: get the vector contains MagicWords
		// Parameters:
		//     none. 
		// Return : the vector contains MagicWords.
		vector<MagicWord> getVecWord(){
			return vecWord;
		}
		
		// Summary: get the total string
		// Parameters:
		//     none. 
		// Return : the total string.
		string getTotalStr(){
			return totalStr;
		}
		
		
};

int main(int argc,char* argv[]){
	string inFile;
    string type;  
    // string dbPath;
    // string outFile;  
    // string sep("\t");
    // int col = -1; 

    if(argc > 1){
        type = string(argv[1]);  
    }  
    int ch;  
    opterr = 0; 

    //get the argv from command line
    while ((ch = getopt(argc, argv, "d:i:o:s:c:")) != -1) {  
        switch (ch) {
        	case 'i': 
				inFile = string(optarg); 
				//cout<<"-i is :"<<inFile<<endl;
				break;  
    //         case 'd': 
				// dbPath = string(optarg); 
				// //cout<<"-d is :"<<dbPath<<endl;
				// break;  
            
    //         case 'o':
				// outFile = string(optarg);
				// //cout<<"-o is :"<<outFile<<endl;
				// break;  
    //         case 's': 
				// sep = string(optarg); 
				// //cout<<"-s is :"<<sep<<endl;
				// break;  
    //         case 'c': 
				// col = atoi(optarg); 
				// //cout<<"-c is :"<<col<<endl;
				// break;  
        }
    }
    
    MagicProcess process;
    
    //read a file to process
    process.readFileByStr(inFile);

    //initialize the root node
    Node root(process.getTotalStr(),0);
    
    //sort the vecWord(MagicWords)
    process.sortVecWordDesc();
    
    //find the solution by dfs
    Node maxNode = process.dfsFindMax(root,process.getVecWord());
    
    //print the route of the best node
    process.showAnswer(maxNode);
    
	return 0;
}

