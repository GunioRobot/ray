source ../0sim1-parameters.sh
source ../0parameters.sh
cp $reads reads.fasta
Assemble.pl reads.fasta $wordSize &> log
print-latex.sh $ref reads.fasta.contig EULER-SR