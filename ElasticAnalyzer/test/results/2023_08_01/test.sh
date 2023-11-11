for i in `cat result_version1_ZeroBias.txt  | awk '{ print $3 }' | sort -n | uniq` ; do echo $i ; grep $i result_version1_ZeroBias.txt | wc ; grep $i result_version5_Totem1.txt | wc ; done
