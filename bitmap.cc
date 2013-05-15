#include "bitmap.hh"

using namespace std;
Bitmap::Bitmap(int n_blocks)
    : num_blocks_(n_blocks),
    just_sent_(-1),
    bits_set_(0),
    bitmap_()
{ }

bool Bitmap::is_set( int block_number ){
    if ((bitmap_[block_number/8] & (1 << (block_number%8)) ) == 0) {
        return false;
    }
    else{
        return true;
    }
}

void Bitmap::set_bit( int block_number ){
  if (!is_set(block_number)){
    bits_set_+=1;
    bitmap_[block_number/8] |= (1 << (block_number%8));
  }
}

bool Bitmap::is_full( void ){
   /* for(int i = 0; i < num_blocks_; i++){
        if (is_set(i) == false){
            return false;
        }
    }
    return true;
    */
    return num_blocks_ == bits_set_;
}

int Bitmap::next_block( void ) {
    if (is_full()){
      return -1;
    }
    for( int i=just_sent_+1; ; i++){
        if (is_set(i%num_blocks_) == false){
            just_sent_ = i;
            return i%num_blocks_;
        }
    }
    return -1;
}

