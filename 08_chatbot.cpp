#include <algorithm>
#include <cctype>
#include <iostream>
#include <random>
#include <sstream>
#include <string>
#include <vector>
using namespace std;
struct Intent {vector<string> patterns, responses;};
const vector<Intent> intents = {
    {{"hello","hi","hey","good morning","good evening","good afternoon"},
     {"Hello!","Hi there!","Greetings!","Hey! How can I help you?"}},
    {{"bye","goodbye","see you later","farewell","take care"},
     {"Goodbye!","Take care!","See you later!","Have a great day!"}},
    {{"thank you","thanks","thank you so much","much appreciated","thanks a lot"},
     {"You're welcome!","Glad I could help!","Anytime!","No problem!"}},
    {{"what is your name","who are you","tell me your name"},
     {"I am your friendly chatbot!","You can call me Chatbot!","I am a chatbot created to help you."}},
    {{"what can you do","how can you help me","what do you do"},
     {"I can answer basic questions and chat with you!","I am here to help you.","I can respond to simple queries."}},
    {{"how are you","how are you doing","are you okay"},
     {"I am doing great!","I am feeling helpful today!","I am ready to help you."}},
    {{"how old are you","what is your age","when were you created"},
     {"I do not have an age like humans.","Age is just a number for a chatbot!"}},
    {{"what is the weather","how is the weather today","tell me the weather"},
     {"I cannot check live weather right now.","Please check your weather app for updates."}},
    {{"tell me a joke","make me laugh","tell a joke"},
     {"Why did the computer go to the doctor? It had a virus!","I would tell you a UDP joke, but you might not get it!"}},
    {{"help me","i need help","can you help me"},
     {"Sure! What do you need help with?","Of course! How can I assist you?"}}
};
vector<string> tokens(string s) {
    for (char& ch : s) ch = isalnum((unsigned char)ch) || ch=='_' ?
        (char)tolower((unsigned char)ch) : ' ';
    istringstream in(s); vector<string> result; string word;
    while (in >> word) result.push_back(word);
    return result;
}
int main() {
    mt19937 rng(random_device{}());
    cout << "Chatbot: Hello! Type bye or exit to quit.\n";
    string input;
    while (cout << "You: " && getline(cin,input)) {
        string lower=input;
        transform(lower.begin(),lower.end(),lower.begin(),[](unsigned char c){return tolower(c);});
        if (lower=="bye" || lower=="exit") {cout << "Chatbot: Goodbye!\n"; break;}
        auto words=tokens(input);
        string response="Sorry, I could not understand that.";
        bool found=false;
        for (const Intent& intent : intents) {
            for (const string& pattern : intent.patterns) {
                auto required=tokens(pattern);
                bool matches=all_of(required.begin(),required.end(),[&](const string& w){
                    return find(words.begin(),words.end(),w)!=words.end();
                });
                if (matches) {
                    uniform_int_distribution<int> pick(0,(int)intent.responses.size()-1);
                    response=intent.responses[pick(rng)]; found=true; break;
                }
            }
            if (found) break;
        }
        cout << "Chatbot: " << response << '\n';
    }
}
