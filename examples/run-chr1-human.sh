nohup /software/openmpi-1.5/bin/mpirun -np 22 ../Ray \
 -s /home/boiseb01/nuccore/chr1-10.fa \
 -s /home/boiseb01/nuccore/chr1-1.fa \
 -s /home/boiseb01/nuccore/chr1-2.fa \
 -s /home/boiseb01/nuccore/chr1-3.fa \
 -s /home/boiseb01/nuccore/chr1-4.fa \
 -s /home/boiseb01/nuccore/chr1-5.fa \
 -s /home/boiseb01/nuccore/chr1-6.fa \
 -s /home/boiseb01/nuccore/chr1-7.fa \
 -s /home/boiseb01/nuccore/chr1-8.fa \
 -s /home/boiseb01/nuccore/chr1-9.fa \
 -o /home/boiseb01/nuccore/chr1.fasta > chr1.log &