#include <bitset>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <queue>
#include <stack>
#include <unordered_map>
#include <vector>

template <typename Type> struct BinaryTreeNode {
  BinaryTreeNode *parent = nullptr;
  BinaryTreeNode *left = nullptr;
  BinaryTreeNode *right = nullptr;
  Type data;

  BinaryTreeNode() = default;
  BinaryTreeNode( const Type &_data ) : data( _data ){};
};

class Zip
{
private:
  // Alias and private structure
  using Freq = size_t;
  using Byte = uint8_t;
  using ByteFreqPair = std::pair<Byte, Freq>;
  using Node = BinaryTreeNode<ByteFreqPair>;
  using ByteAddrPair = std::pair<Byte, Node *>;
  using FreqTable = std::unordered_map<Byte, Freq>;
  using AddrTable = std::unordered_map<Byte, Node *>;
  using WritePos = uint8_t;
  struct Compare {
    bool operator()( Node *a, Node *b )
    {
      return a->data.second > b->data.second;
    }
  };


  // Private data
  std::ifstream istrm;
  std::fstream ostrm;
  FreqTable freq_table;
  AddrTable addr_table;
  WritePos trunc = 0;
  Node *root = nullptr;

  // Private function
  void init_freq_table()
  {
    Byte byte = '\0';
    while ( istrm.read( (char *)&byte, 1 ) ) {
      decltype( freq_table.end() ) handler = freq_table.find( byte );
      if ( handler != freq_table.end() )
        ++handler->second;
      else
        freq_table.insert( ByteFreqPair( byte, 1 ) );
    }

    istrm.clear();
    istrm.seekg( 0, std::ios::beg );
  }
  Node *MergeNode( Node *l, Node *r )
  {
    Node *ret =
      new Node( ByteFreqPair( '\0', l->data.second + r->data.second ) );
    ret->left = l;
    ret->right = r;
    l->parent = r->parent = ret;
    return ret;
  }

public:
  void encode()
  {
    Byte byte = '\0';
    Byte code = '\0';
    WritePos begin = 0;
    std::stack<bool> st;

    while ( istrm.read( (char *)&byte, 1 ) ) {
      // Find node
      auto result = addr_table.find( byte );
      if ( result == addr_table.end() )
        return;
      // Get code
      auto walk = result->second;
      while ( walk->parent ) {
        ( walk == walk->parent->left ) ? st.push( 0 ) : st.push( 1 );
        walk = walk->parent;
      }

      // Write code to buffer in binary
      // WritePos len = ( st.size() + 7 ) & ~7;
      // WritePos trunc = st.size() + 8 - len;

      while ( !st.empty() ) {
        WritePos mask = 0;
        do {
          mask = 1 << begin;
          if ( st.top() )
            code |= mask;
          st.pop();
          begin = ( begin + 1 ) % 8;
        } while ( !st.empty() && begin );
        if ( !begin ) {
          ostrm << code;
          code = '\0';
        }
      }
    }
    if ( begin )
      ostrm << code;
    trunc = ( begin ) ? begin : 8;

    istrm.clear();
    ostrm.clear();
    istrm.seekg( 0, std::ios::beg );
    ostrm.seekp( 0, std::ios::beg );
  }

  void decode()
  {
    Byte byte = '\0';
    Node *walk = root;
    while ( ostrm.read( (char *)&byte, 1 ) ) {
      WritePos cnt = ( ostrm.peek() == -1 ) ? trunc : 8;
      while ( cnt-- ) {
        walk = ( byte & 1 ) ? walk->right : walk->left;
        if ( walk->left == walk->right ) {
          std::cout << walk->data.first;
          walk = root;
        }
        byte >>= 1;
      }
    }
    ostrm.clear();
    ostrm.seekg( 0, std::ios::beg );
  }
  void init_huffman_tree()
  {
    std::priority_queue<Node *, std::vector<Node *>, Compare> heap;

    for ( auto &r : freq_table ) {
      Node *t = new Node( r );
      heap.push( t );
      addr_table.insert( ByteAddrPair( r.first, t ) );
    }

    Node *tmp = nullptr;
    while ( heap.size() > 1 ) {
      Node *l = nullptr, *r = nullptr;
      l = heap.top();
      heap.pop();
      if ( !heap.empty() ) {
        r = heap.top();
        heap.pop();
        tmp = MergeNode( l, r );
        heap.push( tmp );
      }
    }

    if ( !heap.empty() )
      root = heap.top();
  }

  Zip() = default;
  Zip( const char *ifname, const char *ofname,
       std::ios_base::openmode imode = std::ios_base::in,
       std::ios_base::openmode omode = std::ios_base::in | std::ios_base::out |
                                       std::ios_base::trunc )
    : istrm( ifname, imode ), ostrm( ofname, omode )
  {
    init_freq_table();
    init_huffman_tree();
  };
  Zip( const std::string &ifname, const std::string &ofname,
       std::ios_base::openmode imode = std::ios_base::in,
       std::ios_base::openmode omode = std::ios_base::in | std::ios_base::out |
                                       std::ios_base::trunc )
    : istrm( ifname, imode ), ostrm( ofname, omode ){};
};

int main( int argc, char *argv[] )
{
  if ( argc > 1 ) {
    Zip zip( argv[1], argv[2] );
    zip.encode();
    zip.decode();
  }
  return 0;
}
