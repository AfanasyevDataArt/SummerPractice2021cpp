#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <chrono>

const size_t NUMBER_OF_THREADS = 4;

using namespace std;

string repl = "                                                1234567890:;<=>?@abcdefghijklmnopqrstuvwxyz[\\]^_`ABCDEFGHIJKLMNOPQRSTUVWXYZ";

void process_line(string &out_line){

    for(size_t i=0; i<out_line.size(); i++){
        out_line[i] = repl[(int)(out_line[i])];
    }

}

void process_thread(vector<string> &lines){
    for(size_t i=0; i<lines.size(); i+=1){
        process_line(lines[i]);
    }
}

int main() {
    repl[10] = (char)(10); repl[13] = (char)(13);
    chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    chrono::steady_clock::time_point data_load, data_process;
    try{
        cout<<"Load input data"<<endl;
        ifstream in_file("../input.data");

        if(!in_file.is_open()){
            cout<<"Fail to open file!"<<endl;
            return 0;
        }

        vector<string> lines;
        string buffer;
        while(in_file >> buffer){
            lines.push_back(buffer+"\n");
        }
        in_file.close();

        data_load = std::chrono::steady_clock::now();

        process_thread(ref(lines));

        data_process = std::chrono::steady_clock::now();

        cout<<"Save output data"<<endl;
        ofstream out_file("../output_cpp.data");
        for(auto &line : lines){
            out_file.write(line.c_str(), line.size());
        }
        out_file.close();

        cout<<"Done"<<endl;
    }catch (const exception& e){
        cout<<e.what();
    }

    chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

    std::cout << "data load time: " << std::chrono::duration_cast<std::chrono::milliseconds>(data_load - begin).count() << "ms" << std::endl;
    std::cout << "data process time: " << std::chrono::duration_cast<std::chrono::milliseconds>(data_process - data_load).count() << "ms" << std::endl;
    std::cout << "data save time: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - data_process).count() << "ms" << std::endl;
    std::cout << "total execution time: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "ms" << std::endl;
    return 0;
}
