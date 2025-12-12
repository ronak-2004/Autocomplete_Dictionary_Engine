#include <bits/stdc++.h>
using namespace std;

static inline string normalize(const string &s) {
    string t;
    t.reserve(s.size());
    for( char c:s){
        if(isalpha((unsigned char)c)) { 
            t.push_back(char(tolower((unsigned char)c)));
        }
    }
    return t;
}

struct TrieNode {
    bool isWord = false;
    int freq = 0;
    array<unique_ptr<TrieNode>,26> child{};
    TrieNode() = default;
};

class Trie {
    unique_ptr<TrieNode>root;
  public:
    Trie(){root = make_unique<TrieNode>();}

    void insert(const string &word, int count = 1) {
        string s = normalize(word);
        if(s.empty()) return;
        TrieNode* cur = root.get();
        for(char c: s){
            int index = c-'a';
            if(cur->child[index]==nullptr){
                cur->child[index] = make_unique<TrieNode>();
            }
            cur = cur->child[index].get();
        }
        cur->isWord = true;
        cur->freq += count;
    }

    int search(const string &word) const{
        string s = normalize(word);
        if(s.empty()) return 0;
        const TrieNode* cur = root.get();
        for(char ch:s){
            int idx = ch - 'a';
            if(!cur->child[idx]) return 0;
            cur = cur->child[idx].get();
        }
        return cur->isWord ? cur->freq : 0;
    }

    TrieNode* findNode(const string &word) const{
        string s = normalize(word);
        if(s.empty()) return root.get();
        TrieNode* cur = root.get();
        for(char ch:s) {
            int idx = ch - 'a';
            if(!cur->child[idx]) return nullptr;
            cur = cur->child[idx].get();
        }
        return cur;
    }

    void collectAll(TrieNode* node, string &prefix, vector<pair<string,int>>&out) const {
        if(!node) return;
        if(node->isWord) out.push_back({prefix,node->freq});
        for(int i=0;i<26;i++){
            if(node->child[i]) {
                prefix.push_back(char('a'+i));
                collectAll(node->child[i].get(),prefix,out);
                prefix.pop_back();
            }
        }
    }

    vector<pair<string,int>> suggestions(const string &prefix) const {
        string p = normalize(prefix);
        vector<pair<string,int>> res;
        if(p.empty()) return res;
        TrieNode* node = findNode(p);

        if(!node) return res;

        string temp = p;
        collectAll(node,temp,res);
        sort(res.begin(),res.end(),[](auto &a,auto &b){
            if(a.second != b.second) return a.second > b.second;
            return a.first < b.first;
        });
        return res;
    }

    bool remove(const string &word) {
        string s = normalize(word);
        if(s.empty()) return false;
        return removeRec(root.get(),s,0);
    }
  private:
    bool isEmpty(TrieNode* node){
        for(int i=0;i<26;i++) 
            if(node->child[i]) return false;
        return true;
    }

    bool removeRec(TrieNode* node, const string &s,size_t pos){
        if(!node) return false;
        if(pos==s.size()) {
            if(!node->isWord) return false;
            node->isWord = false;
            node->freq = 0;
            return true;
            //logic to delete current node (taken care recursively by parent)
        }
        int idx = s[pos]-'a';
        if(!node->child[idx]) return false;
        bool childResult = removeRec(node->child[idx].get(),s,pos+1);
        TrieNode* child = node->child[idx].get();
        if(childResult && isEmpty(child) && !child->isWord){
            node->child[idx].reset();
        }
        return childResult;
    }
};

void print_help() {
    cout<<"Commands: \n"
        << " load <filename>      - load words from file (one word per line or whitespace separated)\n"
        << " add <word>           - insert word (freq++ if exists)\n"
        << " query <prefix>       - list suggestions for prefix (sorted by frequency)\n"
        << " delete <word>        - delete word from trie\n"
        << " search <word>        - search a specific word and show its frequency\n"
        << " search               - show all words stored in trie with frequencies\n"
        << " help                 - show this help\n"
        << " exit                 - quit\n";
}

int main(int argc, char** argv) {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    Trie trie;

    vector<string> sample = {
        "apple", "apply", "apricot", "banana", "band", "bandana",
        "can", "candy", "cap", "cape", "apple", "apple", "apply"
    };

    for(auto &w: sample) trie.insert(w);
    auto load_file = [&](const string &fname){
        ifstream fin(fname);
        if(!fin) {
            cerr<<"Could not open "<<fname<<"\n";
            return;
        }
        string w;
        long long cnt=0;
        while(fin>>w){
            trie.insert(w);
            cnt++;
        }
        cout<<"Loaded "<<cnt<<" words from "<<fname<<"\n";
    };
    if(argc >= 2) load_file(argv[1]);

    cout<< "Autocomplete Trie ready. Type 'help' for commands.\n";
    print_help();

    string cmd;
    while(true){
        cout<<"> "<<flush;
        if(!(cin>>cmd))break;
        if(cmd == "load") {
            string fname; 
            cin>>fname;
            load_file(fname);
        }
        else if(cmd == "add") {
            string w;
            cin>>w;
            trie.insert(w);
            cout<<"Added / incremented: "<<normalize(w)<<"\n";
        }
        else if(cmd == "query") {
            string pref;
            cin>>pref;
            auto res = trie.suggestions(pref);
            if(res.empty()){
                cout<<"No suggestions for \""<<pref<<"\"\n";
                
            }
            else{
                cout<<"Suggestions:\n";
                for(auto &p: res) cout<<" "<<p.first<<" ("<<p.second<<")\n";
            }
        }
        else if(cmd == "delete") {
            string w;
            cin>>w;
            bool ok = trie.remove(w);
            cout<<(ok?"Deleted ":"Not found ")<<normalize(w)<<"\n";
        }
        else if(cmd == "search") {
            string restOfLine;
            getline(cin,restOfLine);
            if(restOfLine.empty()){
                vector<pair<string,int>>all;
                string temp="";
                trie.collectAll(trie.findNode(""),temp,all);
                sort(all.begin(),all.end(),[](auto &a,auto &b){
                    if(a.second != b.second) return a.second > b.second;
                    return a.first < b.first;
                });
                cout<<"All words:\n";
                for(auto &p:all){
                    cout<<" "<<p.first<<" ("<<p.second<<")\n";
                }
            }
            else{
                restOfLine = normalize(restOfLine);
                int f = trie.search(restOfLine);
                if(f) cout<< normalize(restOfLine)<<" found with frequency "<<f<<"\n";
                else cout<<normalize(restOfLine)<<"  Not found\n";
            }
        }
        else if(cmd == "help") {
            print_help();
        }
        else if(cmd == "exit") {
            break;
        }
        else {
            cout<<"Unknown command. Type 'help' for list.\n";
        }
    }
    cout<<"Bye\n";
    return 0;
}