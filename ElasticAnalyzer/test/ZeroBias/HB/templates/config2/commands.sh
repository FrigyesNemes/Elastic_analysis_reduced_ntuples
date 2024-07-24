for i in `cat /afs/cern.ch/work/f/fnemes/main_workspace_github_ssh_4/Projects/TOTEM_Projects/Physics_projects/Physics_analysis/Proton_proton_scattering/Elastic_scattering/Projects/2023/E_CM_900_GeV_beta_star_100_m/General_settings/List_of_runs.txt`
do
	echo $i
	minimum=`grep event_info_timestamp_min ../../tmp/results\_$i.txt | awk '{ print $2 }'`
	maximum=`grep event_info_timestamp_max ../../tmp/results\_$i.txt | awk '{ print $2 }'`

	sed -isave "s/\"timestamp_minimum\"/$minimum/g" cfg_analyzer_template_run\_$i\_re_reco_ZeroBias.py
	sed -isave2 "s/\"timestamp_maximum\"/$maximum/g" cfg_analyzer_template_run\_$i\_re_reco_ZeroBias.py
done
