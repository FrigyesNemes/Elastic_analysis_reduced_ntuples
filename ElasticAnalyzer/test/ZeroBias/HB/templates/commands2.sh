for i in `cat /afs/cern.ch/work/f/fnemes/main_workspace_github_ssh_4/Projects/TOTEM_Projects/Physics_projects/Physics_analysis/Proton_proton_scattering/Elastic_scattering/Projects/2023/E_CM_900_GeV_beta_star_100_m/General_settings/List_of_runs.txt`
do
	cat cfg_analyzer_template_run_XXXX_re_reco_ZeroBias_header.py > config2/cfg\_analyzer\_template\_run\_$i\_re\_reco\_ZeroBias.py
	eos ls /eos/totem/data/offline/2018/450GeV/beta100/ZeroBias/version5/ | grep root | grep $i > tmp/files.txt
	cat tmp/files.txt | sed "s#run#\"root://eostotem//eos/totem/data/offline/2018/450GeV/beta100/ZeroBias/version5/run#g" | sed "s#\.root#.root\",#g" >> config2/cfg\_analyzer\_template\_run\_$i\_re\_reco\_ZeroBias.py 
	cat cfg_analyzer_template_run_XXXX_re_reco_ZeroBias_footer.py >> config2/cfg\_analyzer\_template\_run\_$i\_re\_reco\_ZeroBias.py 
done

