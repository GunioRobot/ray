CODE=../code

g++ $CODE/structures/Direction.cpp $CODE/structures/ReadAnnotation.cpp $CODE/memory/malloc_types.cpp  $CODE/structures/Vertex.cpp test_kmer.cpp  $CODE/core/common_functions.cpp $CODE/structures/Kmer.cpp $CODE/cryptography/crypto.cpp -I$CODE -I. -D MAXKMERLENGTH=32 -DASSERT -I..
./a.out TGAAATGGAAATGGTCTGGGAAG

g++ $CODE/structures/Direction.cpp  $CODE/structures/ReadAnnotation.cpp $CODE/memory/malloc_types.cpp  $CODE/structures/Vertex.cpp test_kmer.cpp  $CODE/core/common_functions.cpp $CODE/structures/Kmer.cpp $CODE/cryptography/crypto.cpp -I$CODE -I. -D MAXKMERLENGTH=64 -DASSERT -I..
./a.out \
TGAAATGGAAATGGTCTGGGAAAAACAACTAAAAGATATTATTGTAGTA

g++ $CODE/structures/Direction.cpp  $CODE/structures/ReadAnnotation.cpp $CODE/memory/malloc_types.cpp  $CODE/structures/Vertex.cpp test_kmer.cpp $CODE/core/common_functions.cpp  $CODE/structures/Kmer.cpp $CODE/cryptography/crypto.cpp -I$CODE -I. -D MAXKMERLENGTH=96 -DASSERT -I..
./a.out \
TGAAATGGAAATGGTCTGGGAAAAACAACTAAAAGATATTATTGTAGTAGCTGGTTTTGAAATTTATGACGCTGAAATAACTCCCCACTA

g++ $CODE/structures/Direction.cpp  $CODE/structures/ReadAnnotation.cpp $CODE/memory/malloc_types.cpp  $CODE/structures/Vertex.cpp test_kmer.cpp $CODE/core/common_functions.cpp $CODE/structures/Kmer.cpp  $CODE/cryptography/crypto.cpp -I$CODE -I. -D MAXKMERLENGTH=128 -DASSERT -I..
./a.out \
TGAAATGGAAATGGTCTGGGAAAAACAACTAAAAGATATTATTGTAGTAGCTGGTTTTGAAATTTATGACGCTGAAATAACTCCCCACTATATTTTCACCAAATTTATT

g++ $CODE/structures/Direction.cpp  $CODE/structures/ReadAnnotation.cpp $CODE/memory/malloc_types.cpp  $CODE/structures/Vertex.cpp test_kmer.cpp $CODE/core/common_functions.cpp $CODE/structures/Kmer.cpp $CODE/cryptography/crypto.cpp  -I$CODE -I. -D MAXKMERLENGTH=75 -DASSERT -I..
./a.out \
TGAAATGGAAATGGTCTGGGAAAAACAACTAAAAGATATTATTGTAGTAGCTGGTTTTGAAATTTATGACGCT

g++ $CODE/structures/Direction.cpp  $CODE/structures/ReadAnnotation.cpp $CODE/memory/malloc_types.cpp  $CODE/structures/Vertex.cpp test_kmer.cpp $CODE/core/common_functions.cpp $CODE/structures/Kmer.cpp $CODE/cryptography/crypto.cpp  -I$CODE -I. -D MAXKMERLENGTH=50 -DASSERT -I..
./a.out \
TGAAATGGAAATGGTCTGGGAAAAACAACTAAAAGATATTAT
