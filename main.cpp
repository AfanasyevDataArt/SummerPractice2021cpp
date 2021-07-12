#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <fstream>
#include <algorithm>
#include <chrono>

const size_t NUMBER_OF_THREADS = 4;

using namespace std;

string process_line(string in_line){
    string out_line=in_line;

    for(size_t i=0; i<out_line.size(); i++){
        //if-else конструкции трудней оптимизируются процессором.
        //Быстрей всего будет работать таблица преобразование как тут, https://github.com/slavavrn/SummerPractice2021cpp/blob/main/main.cpp#L13
        // но лучше было использовать память на стеке. string выделяет память в куче
        /*
        string repl = "                                                1234567890:;<=>?@abcdefghijklmnopqrstuvwxyz[\\]^_`ABCDEFGHIJKLMNOPQRSTUVWXYZ";

        void process_line(string &out_line){

            for(size_t i=0; i<out_line.size(); i++){
                out_line[i] = repl[(int)(out_line[i])];
            }

        }
*/
        //Тернарный оператор (a>b?1:0) не даст вам прироста производительности перед if-else
        //Они компелируются в один и тотже ассемблеровский код -> https://godbolt.org/z/EEW3r4TGP

        if (isdigit(out_line[i])){
            int d = out_line[i] -'0';
            if(d < 9){
                out_line[i] = '0' + d + 1;
            }else{
                out_line[i] = '0';
            }
        }else if (isupper(out_line[i])){
            out_line[i] = tolower(out_line[i]); tolower(out_line[i]);
        }else{
            out_line[i] = toupper(out_line[i]);
        }
    }

    return out_line;
}

void process_thread(vector<string>& lines, size_t offset, size_t number_of_threads){
    //Каждый поток читат строки перепрыгивая через 3. Это не cache-friendly
    //Лучше когда каждый поток работает с непрерывным массмивом памяти.
    //Это позволяет процессору заранее подтягивать данные из ОЗУ в кэш память
    for(size_t i=offset; i<lines.size(); i+=number_of_threads){
        lines[i] = process_line(lines[i]); //при вызове process_line происходит копирование строки
    }
}

int main() {
    chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    chrono::steady_clock::time_point data_load;
    try{
        cout<<"Load input data"<<endl;
        ifstream in_file("../input.data");

        if(!in_file.is_open()){
            cout<<"Fail to open file!"<<endl;
            return 0;
        }

        //mmap() позволяет работать с файлом как с буфером в памяти. Это намного быстрее простых read потому что
        //не далает системых вызовов и не копирует данные.

        //Объект string предславляет собой служебные данные и указатель на бухер с полезными данными.
        // То есть каждая строчка лежит в памяти отдельно, что тоже не cache-friendly
        // Лучше было бы использовать std::array<char, LINE_LENGTH> или просто char[LINE_LENGTH] вместо string
        vector<string> lines;

        string buffer;
        while(in_file >> buffer){
            //Тут происходит копирования строчки.
            //У vector  есть внутренне хранилище. Когда мы делаем push_back() и хранилище заканчивается,
            // Выделяется память в 2 раза больше и  все данные туда перекопируются. Если у нас в векторе лежит 256МБ и
            // мы хотим туда что-то положить переде этим вызовется выделение 512МБ и копирование 256МБ, что может занят время.
            // Можно выделить сразу место для всего файла через .resize()
            lines.push_back(buffer+"\n");
        }
        in_file.close();

        data_load = std::chrono::steady_clock::now();

        cout << "Start " << NUMBER_OF_THREADS << " threads" << endl;
        vector<thread> thread_pool;
        for(size_t i=0; i<NUMBER_OF_THREADS; i++){
            thread_pool.emplace_back(process_thread, ref(lines), i ,NUMBER_OF_THREADS);
        }

        cout<<"Wait for finish"<<endl;
        for(auto& th : thread_pool){
            th.join();
        }

        //Сохранять файл  можно тоже без mmap
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
    std::cout << "total execution time: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "ms" << std::endl;
    return 0;
}
