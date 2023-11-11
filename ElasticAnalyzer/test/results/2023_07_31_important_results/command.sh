echo "Processing RP"$1
for i in `cat truly_common_events_filtered_sorted.txt` ; do grep $i results_Totem1_version5.txt ; echo ; grep $i results_ZeroBias_version1.txt ; echo ; done | grep "rpDecId $1 " | sed "s/:/ /g"
echo ""
