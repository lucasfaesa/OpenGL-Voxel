#include <iostream>
#include <fstream> 
using namespace std;

class Logger{

    ofstream   handle;

    public:
        Logger(const string& filename):handle(filename){
            if(!handle.is_open()){
                throw runtime_error("Could not open log file");
            }
        }
        
        ~Logger(){
            cout << "Logger destroyed";
            handle.close();
        }

        void log(const string& message){
            if(handle.is_open()){
                handle << message << endl;
            }
        }
};

int main()
{
    cout<< "Hello, log something!" << endl;

    string input;
    cin >> input;

    {
        Logger logger = Logger("yo.txt");
        
        logger.log(input);
        cout << "Logged succesfully" << endl;
    }


    cin.get();

    return 0;
}

