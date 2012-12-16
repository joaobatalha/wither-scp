#include "bitmap.hh"

using namespace std;
Bitmap::Bitmap(int n_blocks)
    : num_blocks_(n_blocks),
    bitmap_()
{ }

Bitmap::is_set( int block_number ){
    if (bitmap[block_number/8] & (1 << (block_number%8)) ) == 0{
        return false;
    }
    else{
        return true;
    }
}

Bitmap::set_bit( int block_number ){
    bitmap[block_number/8] |= (1 << (block_number%8));
}

Bitmap::is_full( void ){
    for(int i = 0; i < n_blocks; i++){
        if (is_set(i) == false){
            return false;
        }
    }
    return true;
}

Bitmap::next_block( void ){
    for( int i=0; i < n_blocks; i++){
        if (is_set(i) == false){
            return i;
        }
    }
    return -1;
}

