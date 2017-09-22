versions=('2016' '2016.5' '2017' '2018')
op_system='mac'
plugin_name='clonerMulti'
make_dir='/Users/hunyadijanos/Documents/GIT/cc-maya-instancer_main/make/build/'$op_system'/'
destination_dir='/Users/hunyadijanos/Documents/GIT/cc-maya-instancer_main/build/'$op_system'/'


for i in "${versions[@]}"
	do
		
		if [ -e $make_dir$i ]
		then
			# echo $destination_dir$i
			cmake --build $make_dir$i --config Release
			
			bundle_file=$make_dir$i'/src/Release/'$plugin_name'.bundle'
			
			
			if [ -e $bundle_file ]
			then
				mkdir -p $destination_dir$i
				
				cp $bundle_file $destination_dir$i

			fi
			
		fi
		
	done

