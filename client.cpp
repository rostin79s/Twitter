#include <iostream>
#include <string>
#include <grpcpp/grpcpp.h>
#include "twitter.grpc.pb.h"


using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

using twitter::s_register;
using twitter::user_request;
using twitter::user_reply;
using twitter::login_reply;
using twitter::login_request;
using twitter::tweet_request;
using twitter::tweet_reply;

using namespace std;





class register_client {
    public:
        register_client(std::shared_ptr<Channel> channel) : stub_(s_register::NewStub(channel)) {}

    string register_user(string username,string password) {
        user_request request;

        request.set_username(username);
        request.set_password(password);

        user_reply reply;

        ClientContext context;

        Status status = stub_->register_user(&context, request, &reply);

        if(status.ok()){
            return reply.res_register();
        } else {
            cout << status.error_code() << ": " << status.error_message() << endl;
            return "-1";
        }
    }

    string login_user(string username,string password){
        login_request request;

        request.set_username(username);
        request.set_password(password);

        login_reply reply;

        ClientContext context;

        Status status = stub_->login_user(&context, request, &reply);

        if(status.ok()){
            return reply.res_login();
        } else {
            cout << status.error_code() << ": " << status.error_message() << endl;
            return "-1";
        }
    }

    string tweet(string username,string command){
        tweet_request request;
        

        if(command=="write_tweet"){
            string tweet;
            cout<<"write your tweet: "<<endl;
            getline(cin,tweet);
            request.set_tweet(tweet);
            request.set_username(username);
            request.set_command(command);
        }
        else if(command=="show_tweets"){
            request.set_username(username);
            request.set_command(command);
        }
        else if(command=="like_tweet"){
            cout<<"which tweet would you want to like? "<<endl;
            string id;
            cin>>id;
            request.set_username(username);
            request.set_id(id);
            request.set_command(command);
        }
        else if(command=="show_likes"){
            cout<<"which tweet would you like to see the list of likes?";
            string id;
            cin>>id;
            request.set_username(username);
            request.set_id(id);
            request.set_command(command);
        }
        else if(command=="retweet"){
            cout<<"which tweet would you like to retweet?";
            string id;
            cin>>id;
            request.set_username(username);
            request.set_id(id);
            request.set_command(command);
        }



        tweet_reply reply;

        ClientContext context;
        Status status= stub_->tweet(&context,request,&reply);
        if(status.ok()){
            return reply.res_tweet();
        } else {
            cout << status.error_code() << ": " << status.error_message() << endl;
            return "-1";
        }
    }

    private:
        std::unique_ptr<s_register::Stub> stub_;
};









void Run() {
    string address("0.0.0.0:5000");
    register_client client(
        grpc::CreateChannel(
            address, 
            grpc::InsecureChannelCredentials()
        )
    );

    string response;
    string username,password;
    cout<<"do you want to login or register? ";
    string str1;
    cin>>str1;

    if(str1=="login"){
        cout<<"username: ";
        cin>>username;
        cout<<"password: ";
        cin>>password;
        response = client.login_user(username,password);
        if(response=="ok"){
            cout<<endl<<"you have succesfully logged in"<<endl;
        }
        else{
            cout<<response<<endl;
            Run();
        }
    }
    else if(str1=="register"){
        cout<<"username: ";
        cin>>username;
        cout<<"password: ";
        cin>>password;
        response = client.register_user(username,password);
        if(response=="ok"){
            cout<<"your account has been registered"<<endl;
            Run();
        }
        else{
            cout <<  response << endl;
            Run();
        }
    }
    else{
        cout<<"please try again"<<endl;
        Run();
    }
    cout<<endl<<"what would you like to do? "<<endl;
    cout<<endl<<"1.tweet    "<<"2.show all tweets    "<<"3.like a tweet     "<<"4.show likes of a tweet   "<<"5.retweet a tweet    "<<"6.logout"<<endl;
    string x="0";
    while(x!="6"){
        getline(cin,x);
        if(x=="1"){
            response = client.tweet(username,"write_tweet");
            if(response!="ok"){
                cout<<endl<<response<<endl;
            }
        }
        else if(x=="2"){
            cout<<endl;
            response=client.tweet(username,"show_tweets");
            cout<<endl<<response<<endl;
            
        }
        else if(x=="3"){
            response=client.tweet(username,"like_tweet");
            cout<<endl<<response<<endl;
        }
        else if(x=="4"){
            response=client.tweet(username,"show_likes");
            cout<<endl<<response<<endl;
        }
        else if(x=="5"){
            response=client.tweet(username,"retweet");
            cout<<endl<<response<<endl;
        }
    }

}









int main(int argc, char* argv[]){
    Run();
    

    return 0;
}