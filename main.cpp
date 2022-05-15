#include "ram.h"
#include "cache.h"
cache A;
ram B;
void test_cache(){
    A.print_cache();
    cout << "\n \n";
    
    // read from index 1, tag 1
    int x = A.cache_read(0b100000001);
    cout << "data is: " << x << endl;
    A.print_cache();
    cout << "\n \n";

    // read from index 1, new tag (20)
    x = A.cache_read(0b1010000000001);
    cout << "data is: " << x << endl;
    A.print_cache();
    cout << "\n \n";
    
    // write to index 2, tag 2
    A.cache_write(0b1000000010, 999);
    A.print_cache();
    cout << "\n \n";

    // write from index 2, new tag 21
    A.cache_write(0b1010100000010, 999);
    A.print_cache();
    cout << "\n \n";

    B.print_ram();
    A.cache_write(0b1111100000010, 333);
    A.print_cache();
    cout << "\n \n";
    x = A.cache_read(0b1110000000010);
    A.print_cache();
    B.print_ram();



}


int main()
{
    test_cache();
    return 0;
}