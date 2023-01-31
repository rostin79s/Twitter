#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
#include <ctime>
#include <grpcpp/grpcpp.h>
#include "twitter.grpc.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;

using twitter::s_register;
using twitter::user_request;
using twitter::user_reply;
using twitter::login_reply;
using twitter::login_request;
using twitter::tweet_request;
using twitter::tweet_reply;

using namespace std;


bool numeric(string str){
    for(int i=0;i<str.size();i++){
        if(str[i]<48 || str[i]>57){
            return false;
        }
    }
    return true;
}


void lower(string str){
    transform(str.begin(), str.end(), str.begin(), ::tolower);
}


class shell{
    public:
        int count;
        vector <vector<string>> text;
        string lower(string str){
            transform(str.begin(), str.end(), str.begin(), ::tolower);
            return str;
        }
        
        void schema(){
            ifstream file1("schemas.txt");
            string line;
            count=0;
            while(getline(file1,line)){
                count+=1;
            }
            text.resize(count);
            for(int i=0;i<count;i++){
                text[i].resize(count);
            }
            file1.close();
            int x=0;
            int y=0;
            ifstream file("schemas.txt");
            while(getline(file,line)){
                if(isspace(line[0])){
                    x=x+1;
                    y=0;
                }
                else{
                    text[x][y]=line;
                    y=y+1;
                }
            }
            file.close();
        }


        vector <string> schema_val(int index ,int j){
            vector <string> text_line(3);
            int c=0;
            int k=0;
            string temp="";
            while(true){
                if(isspace(text[index][j][k])){
                    text_line[c]=temp;
                    c=c+1;
                    k=k+1;
                    temp="";
                    if(c==3){
                        break;
                    }
                }   
                else{
                    temp+=text[index][j][k];
                    k=k+1;
                }
            }
            return text_line;
        }

        void insert(string com){
            if(com.size()<=10){
                cout<< "the command you wrote is wrong,please try again";
            }
            int x=0;
            while(com[x]==32){
                x=x+1;
            }
            string insert=lower(com.substr(x,6));
            if(insert=="insert"){
                x=x+6;
                if(com[x]!=32){
                    cout<< "the command you wrote is wrong,please try again";
                }
                while(com[x]==32){
                    x=x+1;
                }
                string into=lower(com.substr(x,4));
                if(into=="into"){
                    x=x+4;
                    if(com[x]!=32){
                    cout<< "the command you wrote is wrong,please try again";
                    }
                    while(com[x]==32){
                        x=x+1;
                    }
                    int k=x;
                    while(com[k]!=32){
                        k=k+1;
                    }

                    string tb_name=lower(com.substr(x,k-x));
                    for(int i=0;i<count;i++){
                        if(tb_name==lower(text[i][0])){
                            if(com[k]!=32){
                            cout<< "the command you wrote is wrong,please try again";
                            }
                            while(com[k]==32){
                                k=k+1;
                            }   
                            string values=lower(com.substr(k,6));
                            if(values=="values"){
                                string values[count];
                                k=k+6;
                                while(com[k]==32){
                                    k=k+1;
                                }
                                if(com[k]!='('){
                                    cout<< "the command you wrote is wrong,please try again";
                                }
                                k=k+1;
                                string temp="";
                                int j=0;
                                while(com[k]!='('){
                                    while(com[k]!=','){
                                        if(com[k]!=32){
                                            temp+=com[k];
                                        }
                                        k=k+1;
                                    }
                                    values[j]=temp;
                                    j=j+1;
                                }
                                int z=1;
                                while(text[i][z]!=""){
                                    z=z+1;
                                }
                                if(z==j+2){
                                    ofstream file("users.txt");
                                    file << '\n';
                                    for(int k=1;k<z;k++){
                                        vector <string> line_text=schema_val(i,k);
                                        if(line_text[0]=="id"){
                                            continue;
                                        }
                                        for(int c=0;c<j;c++){
                                            if(line_text[1]=="unique"){
                                                
                                            }
                                            else if(line_text[1]=="integer"){
                                                if(numeric(values[c])){

                                                }
                                                else{
                                                    cout<<"number"<<c+1 <<" value type must be integer, please try again";
                                                }
                                            }
                                            else if(line_text[1]=="timestamp"){

                                            }
                                            else if(line_text[1]=="boolean"){

                                            }
                                            else{
                                                
                                            }
                                        }
                                    }
                                }
                                else{
                                    cout<<"number of values is incorrect, please try again";
                                }

                            }
                            else{
                                cout<<"table name does not exist, please try again";
                            }
                        }
                    }
                    cout<<"table name does not exist, please try again";
                }
                else{
                    cout<< "the command you wrote is wrong,please try again";
                }
                
            }
            else{
                cout<< "the command you wrote is wrong,please try again";
            }
        }



};



string login_check(string username,string password){
    ifstream file("users.txt");
    int c=0;
    string line;
    string res;
    while(getline(file,line)){
        if(c==0){
            c=c+1;
            continue;
        }
        else{
            int i=line.find(' ');
            int j=line.find(' ',i+1);
            int k=line.find(' ',j+1);
            if(username==line.substr(i+1,j-i-1)){
                if(password==line.substr(j+1,k-j-1)){
                    return "ok";
                }
            }
        }
    }
    return "username or password is incorrect";
}



void register_to_users(string username,string password,string time){
    ifstream file1("id_users.txt");
    string line;
    getline(file1,line);
    int id=stoi(line);
    file1.close();
    ofstream file2("users.txt",ios::out | ios::app);
    file2<<id<<" "<<username<<" "<<password<<" "<<time;
    file2.close();
    ofstream file3("id_users.txt", ios::out | ios::trunc);
    file3<< id+1<<endl;
    file3.close();
}



bool username_search(string username){
    ifstream file("users.txt");
    string line;
    shell obj;
    int c=0;
    int i=0;
    int k=0;
    vector <string> text_line;
    string temp;
    while(getline(file,line)){
        if(c==0){
            c=c+1;
            continue;
        }
        else{
            while(true){
                if(isspace(line[i])){
                    if(k==1){
                        if(temp==username){
                            return false;
                        }
                        else{
                            break;
                        }
                    }
                    i=i+1;
                    k=k+1;
                    temp="";
                }   
                else{
                    temp+=line[i];
                    i=i+1;
                }
            }
            k=0;
            i=0;
        }
        c=c+1;
    }
    return true;

}





string register_check(string username,string password){
    shell obj;
    obj.schema();
    int i=0;
    
    while(  (obj.text[i][0]).substr(0,5)  !="users"){
        i=i+1;
    }
    int z=1;
    while(obj.text[i][z]!=""){
        z=z+1;
    }
    for(int j=1;j<z;j++){
        vector <string> data= obj.schema_val(i,j);
        if(data[0]=="username"){
            string temp=data[2];
            if(username_search(username)){
                string chr=data[2];
                int start,end;
                for(int i=0;i<chr.size();i++){
                    if(chr[i]=='('){
                        start=i;
                    }
                    else if(chr[i]==')'){
                        end=i;
                    }
                }
                string str=chr.substr(start+1,end-start-1);
                int num=stoi(str);
                if(username.size()>num){
                    return "username has too many characters";
                }
            }
            else{
                return "username has already been taken";
            }
        }
        else if(data[0]=="password"){
            string chr=data[1];
            int start,end;
            for(int i=0;i<chr.size();i++){
                if(chr[i]=='('){
                    start=i;
                }
                else if(chr[i]==')'){
                    end=i;
                }
            }
            string str=chr.substr(start+1,end-start-1);
            int num=stoi(str);
            if(password.size()>num){
                return "password has too many characters";
            }
            
            
        }
    }
    return "ok";

}



string show_tweets(){
    ifstream file("tweets.txt");
    string line;
    string tweets;
    int c=0;
    while(getline(file,line)){
        if(c==0){
            c=c+1;
            continue;
        }
        else{
            int start=0;
            int start_time;
            int k=0;
            string username,retweet,time,tweet,id,likes;
            while(k<9){
                int space=line.find(' ',start);
                if(k==0){
                    id=line.substr(start,space-start);
                }
                else if(k==1){
                    username=line.substr(start,space-start);
                }
                else if(k==2){
                    likes=line.substr(start,space-start);
                }
                else if(k==3){
                    retweet=line.substr(start,space-start);
                    start_time=space+1;
                }
                else if(k==8){
                    time=line.substr(start_time,space-start_time);
                    tweet=line.substr(space+1);
                }
                k=k+1;
                start=space+1;
            }
            if(retweet=="false"){
                tweets+="username="+username+'\n'+id+") tweet: "+tweet+'\n'+'\t'+"number of likes:"+likes+'\t'+time+'\n'+'\n';
            }
            else{
                int x=username.find('_');
                tweets+="username= "+username.substr(0,x)+"   "+"retweeted from: "+username.substr(x+1,username.size()-x-1)+'\n'+id+") tweet: "+tweet+'\n'+'\t'+"number of likes:"+likes+'\t'+time+'\n'+'\n';
            } 
        }
    }
    return tweets;
}


string tweet_check(string tweet){
    shell obj;
    obj.schema();
    int i=0;
    while(i<obj.count){
        if(    (obj.text[i][0]).substr(0,6)   =="tweets"){
            break;
        }
        i=i+1;
    }
    vector <string> values=obj.schema_val(i,6);
    int start=values[1].find('(');
    int end=values[1].find(')');
    string str=values[1].substr(start+1,end-start-1);
    int num=stoi(str);
    if(tweet.size()>num){
        return "too many characters, please try again";
    }
    else{
        return "ok";
    }
}


void register_tweet(string tweet_id ,string username,string retweet){
    if(retweet=="true"){
        ifstream file("tweets.txt");
        string line;
        while(getline(file,line)){
            int space=line.find(' ');
            string id_line=line.substr(0,space);
            if(tweet_id==id_line){
                int space1=line.find(' ',space+1);
                string user=line.substr(space+1,space1-space-1);
                username+="_"+user;
                int start=line.find("2020");
                start+=5;
                tweet_id=line.substr(start,line.size()-start);
                break;
            }
        }
    }
    ifstream file1("id_tweets.txt");
    string line;
    getline(file1,line);
    int id=stoi(line);
    file1.close();
    ofstream file2("tweets.txt",ios::out | ios::app);
    time_t now=time(0);
    string date=ctime(&now);
    string time=date.substr(0,24);
    file2<<id<<" "<<username<<" "<<0<<" "<<retweet<<" "<<time<<" "<<tweet_id<<endl;
    file2.close();
    ofstream file3("id_tweets.txt", ios::out | ios::trunc);
    file3<< id+1<<endl;
    file3.close();

}



void like(string id,string username){
    ifstream file1("tweets.txt");
    string line;
    string text;
    int c=0;
    while(getline(file1,line)){
        if(c==0){
            c=c+1;
            text+=line;
            continue;
        }
        int space=line.find(' ');
        string id_line=line.substr(0,space);
        if(id_line==id){
            int space2=line.find(' ',space+1);
            int space3=line.find(' ',space2+1);
            string num_likes=line.substr(space2+1,space3-space2-1);
            int new_num_likes=stoi(num_likes)+1;
            string new_likes=to_string(new_num_likes);
            string temp=line;
            temp.replace(space2+1,num_likes.size(),new_likes);
            text+='\n'+temp;
        }
        else{
            text+='\n'+line;
        }
    }
    file1.close();
    ofstream file2("tweets.txt");
    file2<<text;
    file2.close();
    ifstream file3("likes.txt");
    int k=0;
    text="";
    while(getline(file3,line)){
        int space=line.find(' ');
        string id_line=line.substr(0,space);
        if(id_line==id){
            text+=line+" "+username+'\n';
            k=1;
        }
        else{
            text+=line+'\n';
        }
                
            
        
    }
    if(k==0){
        ofstream file("likes.txt",ios::out | ios::app);
        file<<endl<<id<<" "<<username;
        file.close();
    }
    else{
        ofstream file("likes.txt");
        file<<text;
    }
    file3.close();

}


bool check_like(string id,string username){
    ifstream file("likes.txt");
    string line;
    while(getline(file,line)){
        int space=line.find(' ');
        string id_line=line.substr(0,space);
        if(id_line==id){
            int i=space+1;
            string temp;
            cout<<line.size()<<endl;
            while(i<line.size()){
                if(!isspace(line[i])){
                    temp+=line[i];
                    i=i+1;
                }
                else{
                    if(temp==username){
                        cout<<line<<endl;
                        return false;
                    }
                    cout<<temp<<endl;
                    temp="";
                    i=i+1;
                    
                }
            }
            if(temp==username){
                return false;
            }
        }
    }
    return true;
}


string show_likes(string id){
    ifstream file("likes.txt");
    string line;
    string likes;
    while(getline(file,line)){
        int space=line.find(' ');
        string id_line=line.substr(0,space);
        if(id_line==id){
            likes=line.substr(space+1,line.size()-1);
        }
    }
    return likes;
}


class twitter_register final : public s_register::Service {
    Status register_user(
        ServerContext* context,
        const user_request* request,
        user_reply* reply
    ) override {
        string username = request->username();
        string password = request->password();
        
        if(register_check(username,password)=="ok"){
            time_t now=time(0);
            string time=ctime(&now);
            register_to_users(username,password,time);
            reply->set_res_register("ok");
        }
        else{
            reply->set_res_register(register_check(username,password));
        }

        return Status::OK;

    }


    Status login_user(
        ServerContext* context, 
        const login_request* request, 
        login_reply* reply
    ) override {
        string username = request->username();
        string password = request->password();
        string response=login_check(username,password);


        reply->set_res_login(response);
        
    

        return Status::OK;

    }

    Status tweet(
        ServerContext* context, 
        const tweet_request* request, 
        tweet_reply* reply
    ) override {
        string username = request->username();
        string command=request->command();
        if(command=="write_tweet"){
            string tweet = request->tweet();
            string response=tweet_check(tweet);
            if(response=="ok"){
                register_tweet(tweet,username,"false");
            }
            reply->set_res_tweet(response);
        }
        else if(command=="show_tweets"){
            string tweets=show_tweets();
            reply->set_res_tweet(tweets);
        }
        else if(command=="like_tweet"){
            string id=request->id();
            bool check=check_like(id,username);
            if(check==true){
                like(id,username);
                reply->set_res_tweet("you have liked the tweet");
            }
            else{
                reply->set_res_tweet("you have already liked the tweet");
            }
        }
        else if(command=="show_likes"){
            string id=request->id();
            string likes=show_likes(id);
            reply->set_res_tweet(likes);
        }
        else if(command=="retweet"){
            string id=request->id();
            register_tweet(id,username,"true");
            reply->set_res_tweet("you have succesfully retweeted the tweet");
        }
        
    

        return Status::OK;

    }
};



void Run() {
    std::string address("0.0.0.0:5000");
    twitter_register service;

    ServerBuilder builder;

    builder.AddListeningPort(address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);

    std::unique_ptr<Server> server(builder.BuildAndStart());
    std::cout << "Server listening on port: " << address << std::endl;

    server->Wait();
}



int main(int argc,char** argv ){
    Run();
    return 0;
}