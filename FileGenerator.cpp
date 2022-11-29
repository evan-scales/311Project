// The following file generator was written entirely by Professor O'Reilly.
// I don't take any credit for the code contained here.

#include <unordered_map>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <random>
#include <vector>
#include <sstream>
#include <chrono>

struct map_op{
    char op;
    int key;
    std::string value;
    std::string result;

    map_op(char op, int key): op(op), key(key) {/**/ }

    map_op(char op, int key, std::string val): op(op), key(key),value(val) {/**/}
};

std::vector<map_op> file2map_ops(std::string fn);
void map_ops_sim(std::vector<map_op>& map_ops);
void map_op_results2file(std::vector<map_op>& map_ops, std::string fn);

void run_st_sim(std::string fn, bool justLoad=false);

bool write_test(std::string fn, const int inserts, const int deletes, const int lookups, int n_threads);

// End header

std::vector<map_op> file2map_ops(std::string fn){
    using namespace std;
    std::vector<map_op> map_ops;

    ifstream in;
    in.open(fn);

    string line;
    getline(in,line);
    map_ops.push_back(map_op('N', stoi(line.substr(2)) ));

    while(getline(in,line)){

        char action;
        int key;
        int first_quote = line.find('"'); 
        
        action = line.at(0);
        key = stoi(line.substr(2,first_quote-2));

        if(action=='I') {
            string rest = line.substr(first_quote,line.length()-first_quote+1);
            map_ops.push_back(map_op(action,key,rest));
        } else{
            map_ops.push_back(map_op(action,key));
        } 
    }
    
    in.close();

    return map_ops;

}

void map_ops_sim(std::vector<map_op>& map_ops){
    using namespace std;
    unordered_map<int,string> dict;

    map_ops[0].result = "Using " + to_string(map_ops[0].key) + " threads";

    for(int i = 1 ; i<map_ops.size(); ++i){
        char action = map_ops[i].op;
        int key = map_ops[i].key;
        
        
        if(action=='I') {
            string value = map_ops[i].value;

            if(dict.find(key)==dict.end()){
                dict.insert({key,value});
                map_ops[i].result = "OK";
            } else{
                map_ops[i].result ="Fail";
            }
        } else if (action =='L'){
            auto search = dict.find(key);
            if(search==dict.end()){
                map_ops[i].result = "No "+ to_string(key);
            } else{
                string value = search->second;
                map_ops[i].result = value.substr(1,value.length()-2); 
            }
        } else if (action == 'D'){
            auto search = dict.find(key);
            if(search==dict.end()){
                map_ops[i].result ="Fail";
            } else{
                dict.erase(key);
                map_ops[i].result ="OK";
            }
        }
    }
    
}

void map_op_results2file(std::vector<map_op>& map_ops, std::string fn){
    using namespace std;
    ofstream out;
    out.open(fn.substr(0,fn.length()-4)+"_out.txt");

    for(int i = 0; i<map_ops.size();++i){
        out<<map_ops[i].result<<endl;
    }

    out.close();
}

void run_st_sim(std::string fn, bool justLoad){
    using namespace std;

    ifstream in;
    ofstream out;

    in.open(fn);
    out.open(fn.substr(0,fn.length()-4)+"_out.txt"); 

    unordered_map<int,string> dict;

    string line;
    getline(in,line);
    out<<"Using " << stoi(line.substr(2))<< " threads"<<endl;

    #ifdef BUFFER_OUT
        stringstream  buff_out;
        const int FLUSH_FREQ = 10000;
        int line_ct = 0;
        #define OUT buff_out
        #define ENDL '\n'
        
    #else
        #define OUT out
        #define ENDL std::endl

    #endif //BUFFER_OUT

    while(getline(in,line)){
        if (justLoad==true) {
                OUT<<"TEST"<<ENDL;
                #ifdef BUFFER_OUT
                    ++line_ct;
                    if(line_ct=FLUSH_FREQ){
                        line_ct=0;
                        out<<buff_out.str();
                        out.flush();
                        buff_out.clear();
                        buff_out.str(std::string());
                    }
                #endif //BUFFER_OUT
                continue;
            }

        char action;
        int key;
        int first_quote = line.find('"');
        
        action = line.at(0);
        key = stoi(line.substr(2,first_quote-2));

        if(action=='I') {
            string rest = line.substr(first_quote,line.length()-first_quote+1);
            if(dict.find(key)==dict.end()){
                dict.insert({key,rest});
                OUT<<"OK"<<ENDL;
            } else{
                OUT<<"Fail"<<ENDL;
            }
        } else if (action =='L'){
            auto search = dict.find(key);
            if(search==dict.end()){
                OUT<<"No "<<key<<ENDL;
            } else{
                string value = search->second;
                OUT<<value.substr(1,value.length()-2)<<ENDL; 
            }
        } else if (action == 'D'){
            auto search = dict.find(key);
            if(search==dict.end()){
                OUT<<"Fail"<<ENDL;
            } else{
                dict.erase(key);
                OUT<<"OK"<<ENDL;
            }
        }

        #ifdef BUFFER_OUT
        ++line_ct;
        if(line_ct=FLUSH_FREQ){
            line_ct=0;
            out<<buff_out.str();
            out.flush();
            buff_out.clear();
            buff_out.str(std::string());
        }
        #endif //BUFFER_OUT

    }
    
    #ifdef BUFFER_OUT
        out<<buff_out.str();
        out.flush();
    #endif //BUFFER_OUT
    

    out.close();
    in.close();
}

bool write_test(std::string fn, const int inserts, const int deletes, const int lookups, int n_threads){
    using namespace std;
    std::ofstream out;
    std::ofstream test_out;

    std::vector<int> insertVec;
    for(int i = 0; i<inserts; ++i){
        insertVec.push_back(i);
    }

    auto rng = std::default_random_engine {};
    std::shuffle(std::begin(insertVec), std::end(insertVec), rng);

    std::vector<int> deleteVec;
    for(int i = 0; i<deletes;++i){
        deleteVec.push_back(rand()%inserts);
    }

    std::vector<int> lookupVec;
    for(int i = 0; i<lookups;++i){
        lookupVec.push_back(rand()%inserts);
    }

    out.open(fn+".txt");
    test_out.open(fn+"_expected.txt");

    out<<"N "<<n_threads<<std::endl;
    test_out<<"Using " << n_threads << " threads"<<std::endl;
    std::unordered_map<int,std::string> dict;

    int n_insert = 0;
    int n_delete = 0;
    int n_lookup = 0;

    for(     ; n_insert < inserts/2 ; ++n_insert){
        int n = insertVec.at(n_insert);
        std::string s = "key"+to_string(n);
        dict.insert({n,s});
        out<<"I "<<n<<" \""<<s<<"\""<<std::endl;
        test_out<<"OK"<<std::endl;

    }
    std::cout<<"in loop"<<std::endl;
    while(n_insert<inserts || n_delete<deletes || n_lookup<lookups){
        int option = rand()%3;
        
        if(option == 0){
            if (n_insert == inserts) continue;
            int n = insertVec.at(n_insert);
            ++n_insert;
            std::string s = "key"+to_string(n);
            
            out<<"I "<<n<<" \""<<s<<"\""<<std::endl;
            if(dict.find(n) == dict.end()){
                dict.insert({n,s});
                test_out<<"OK"<<std::endl;
            } else {
                test_out<<"Fail"<<std::endl;
            }
        } else if (option ==1){
            if(n_delete == deletes) continue;
            int n = deleteVec.at(n_delete);
            ++n_delete;
            out<<"D "<<n<<std::endl;
            if(dict.find(n) == dict.end()){
                test_out<<"Fail"<<std::endl;
            } else{
                dict.erase(n);
                test_out<<"OK"<<std::endl;
            }
        } else{
            if(n_lookup == lookups) continue;
            int n = lookupVec.at(n_lookup);
            ++n_lookup;
            out<<"L "<<n<<std::endl;
            auto search = dict.find(n);
            if(search == dict.end()){
                test_out<<"No "<<n<<std::endl;
            } else{
                test_out<<search->second<<std::endl;
            }
        }
    }

    out.close();
    test_out.close();

    return true;
}

int main(int argc, char *argv[]){
    using namespace std;
    
    if(argc !=5 && argc!= 6){
        cout<<"file_generator expects 4 or 5 arguments"<<endl;
        cout << " the order is <file name without \".txt\"> <num inserts> <num deletes> <num lookups> [<num threads>]"<<endl;
        cout << "if num threads is not given, defaults to 4"<<endl;
        return -1;
    }

    string fn_base = string(argv[1]);
    int n_insert  = atoi(argv[2]);
    int n_delete  = atoi(argv[3]);
    int n_lookup  = atoi(argv[4]);
    int n_threads = argc == 5 ? 5 : atoi(argv[5]);

    write_test(fn_base,n_insert,n_delete,n_lookup,n_threads);
}
