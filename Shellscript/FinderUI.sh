#!/bin/bash
#########################################ui function###########################################
##┌ ┐ ┘ ┐ ━  ─┃┠┨┫┣  / │
upper_dir()
{
	echo [31m
	echo `tput cup 1 23`"      __"
	echo `tput cup 2 23`" /───/ │"
	echo `tput cup 3 23`" │  d  │"
	echo `tput cup 4 23`" └─────┘"
	echo `tput cup 5 23`".."
	echo [0m
}
directory_shape()
{
	filename=`echo "$1" | cut -c11-`
	v1=`expr $2 + 1`; v2=`expr $2 + 2`; v3=`expr $2 + 3`; v4=`expr $2 + 4`
	echo [34m
	echo `tput cup $2 $3`"      __"
	echo `tput cup $v1 $3`" /───/ │"
	echo `tput cup $v2 $3`" │  d  │"
	echo `tput cup $v3 $3`" └─────┘"
	if [ ${#filename} -gt 10 ]; then
			echo `tput cup $v4 $3`" `echo "$filename" | cut -c1-10`.."
	else
			echo `tput cup $v4 $3`" $filename"
	fi
	echo [0m
}
file_shape()
{	
	filename=`echo "$1" | cut -c11-`
	v1=`expr $2 + 1`; v2=`expr $2 + 2`; v3=`expr $2 + 3`; v4=`expr $2 + 4`
	echo `tput cup $v1 $3`" ┌─────┐"
	echo `tput cup $v2 $3`" │  o  │"
	echo `tput cup $v3 $3`" └─────┘"
	if [ ${#filename} -gt 10 ]; then
			echo `tput cup $v4 $3`" `echo "$filename" | cut -c1-10`.."
	else
			echo `tput cup $v4 $3`" $filename"
	fi
}
xfile_shape()
{
	filename=`echo "$1" | cut -c11-`
	v1=`expr $2 + 1`; v2=`expr $2 + 2`; v3=`expr $2 + 3`; v4=`expr $2 + 4`
	echo [31m
	echo `tput cup $v1 $3`" ┌─────┐"
	echo `tput cup $v2 $3`" │  x  │"
	echo `tput cup $v3 $3`" └─────┘"
	if [ ${#filename} -gt 10 ]; then
			echo `tput cup $v4 $3`" `echo "$filename" | cut -c1-10`.."
	else
			echo `tput cup $v4 $3`" $filename"
	fi
	echo [0m
}
sfile_shape()
{
	filename=`echo "$1" | cut -c11-`
	v1=`expr $2 + 1`; v2=`expr $2 + 2`; v3=`expr $2 + 3`; v4=`expr $2 + 4`
	echo [32m
	echo `tput cup $v1 $3`" ┌─────┐"
	echo `tput cup $v2 $3`" │  s  │"
	echo `tput cup $v3 $3`" └─────┘"
	if [ ${#filename} -gt 10 ]; then
			echo `tput cup $v4 $3`" `echo "$filename" | cut -c1-10`.."
	else
			echo `tput cup $v4 $3`" $filename"
	fi
	echo [0m
}
############################################lay out############################################
lay_out()
{
	clear
	#top horizon line
	for((i=0; i<=100; i++))
	do
		echo -n "="
	done
	echo ''
	#left vertical line
	for((i=1; i<=36; i++))
	do
		echo "│"
	done
	#left second vertical line
	for((i=1; i<=28; i++))
	do
		echo `tput cup $i 22`"│"
	done
	#right vertical line
	for((i=1; i<=36; i++))
	do
		echo `tput cup $i 100`"│"
	done
	#middle horizon line
	echo `tput cup 28 1`"******************************************information**********************************************"
	#bottom second horizon line
	echo `tput cup 35 1`"******************************************information**********************************************"
	#bottom horizon line
	echo `tput cup 37 0`"====================================================================================================="
}
					for((i=0; i<3; i++))
					do
						if [ "${copyarr[$i]}" != "" ]; then
							cp ${copyarr_path[$i]}/${copyarr[$i]} `pwd`
							copyarr[$i]=""; copyarr_path[$i]=""
						fi
					done
########################################left side display(upper directorys list)################################################
left_ui()
{
	echo `tput cup 1 1`[31m".."
	dir=`pwd`
	cd ..
	if [ "`ls -l | awk '{print $2}'`" = "0" ]; then
			:
	else
		filearr=($(ls -dl * | grep ^d | awk '{print $1 $9}'; ls -dl * | grep ^- | awk '{print $1 $9}'; ls -dl * | grep -v ^d | grep -v ^- | awk '{print $1 $9}'))
		cd $dir
		i=1
		for var in "${filearr[@]}"
		do
			type=`echo "$var" | cut -c1`
			name=`echo "$var" | cut -c11-`
			i=`expr $i + 1`
			if [ "$type" = "d" ]; then
				if [ ${#name} -gt 10 ]; then
						echo `tput cup $i 1`[34m"`echo "$name" | cut -c1-10`.."; echo [0m
				else
						echo `tput cup $i 1`[34m"$name"; echo [0m
				fi
			elif [ "$type" = "-" ]; then
				if [ ${#name} -gt 10 ]; then
						echo `tput cup $i 1`"`echo "$name" | cut -c1-10`.."
				else
						echo `tput cup $i 1`"$name"
				fi
			else
				if [ ${#name} -gt 10 ]; then
						echo `tput cup $i 1`[32m"`echo "$name" | cut -c1-10`.."; echo [0m
				else
						echo `tput cup $i 1`[32m"$name"; echo [0m
				fi
			fi
		
			if [ $i -eq 21 ]; then
					i=`expr $i + 1`
					echo `tput cup $i 1`"..."
					break
			fi
		done
	fi
	cd $dir
}
#############################################bottom display, current directory information##############################
bottom_ui()
{
	t=`ls -l | wc -l`
	t=`expr $t - 1`
	echo `tput cup 36 19`"$t total"
	declare -i dir_count=`ls -l | grep ^d | wc -l`; echo `tput cup 36 31`"$dir_count dir"
	declare -i file_count=`ls -l | grep ^- | wc -l`; echo `tput cup 36 43`"$file_count file"
	declare -i sfile_count=`ls -l | grep -v ^d | grep -v ^- | wc -l`
	sfile_count=`expr $sfile_count - 1`
	echo `tput cup 36 55`"$sfile_count sfile"
	echo `tput cup 36 67`"`du -sb | awk '{print $1}'` byte"
}
#############################################중앙 ui 현재 디렉토리 파일 출력################################################
index=0
middle_ui()
{
	if [ "`ls -l | awk '{print $2}'`" = "0" ]; then
		arr=("cd ..")
	else
		arr=("cd .." $(ls -dl * | grep ^d | awk '{print $1 $9}'; ls -dl * | grep ^- | awk '{print $1 $9}'; ls -dl * | grep -v ^d | grep -v ^- 	| awk '{print $1 $9}'))
	fi	
		count=0; width=37; height=1;
		declare -i total=`ls -l | wc -l`
		total=`expr $total - 1`

		total=`expr $total + 1`
		if [ "$1" = "" ]; then
				:
		else
				index=`expr $index + $1`
		fi
	 	####한 칸 스크롤 된 상태이면 첫번째 자리부터 파일을 출력(상위 디렉토리 폴더 모양 안찍음)
		if [ $index -eq 0 ]; then
				:
		else
				width=23; height=1
		fi

		for((i=$index; i<$total; i++))
		do
	
		####25개 찍히면 탈출
			if [ $count -eq 25 ]; then
				break
			fi
		####한 줄에 5개 찍히면 다음줄로 넘어가기	
			for((j=1; j<5; j++))
			do
				if [ $count -eq `expr 5 \* $j` ]; then
					height=`expr $height + 5`
					width=23
				fi
			done
		####상위 디렉토리 파일 모양 출력
			if [ $i -eq 0 ]; then
				upper_dir	
				count=`expr $count + 1`
				continue
			fi

			check=`echo "${arr[i]}" | cut -c1`

		##########디렉터리 파일모양 출력
			if [ "$check" = "d" ]; then
				directory_shape "${arr[i]}" $height $width
				count=`expr $count + 1`
				width=`expr $width + 14`
				continue	
		###########일반파일 파일모양 출력
			elif [ "$check" = "-" ]; then
		###########실행파일 검출
				check=`echo "${arr[i]}" | cut -c4`
				if [ "$check" = "x" ]; then
					xfile_shape "${arr[i]}" $height $width
					count=`expr $count + 1`
					width=`expr $width + 14`
					continue
				else
					file_shape "${arr[i]}" $height $width
					count=`expr $count + 1`
					width=`expr $width + 14`
					continue
				fi
		##########특수파일 파일모양 출력
			else
				sfile_shape "${arr[i]}" $height $width
				count=`expr $count + 1`
				width=`expr $width + 14`
				continue
			fi
		done	
	
}
information()
{
	file=`echo "$1" | cut -c11-`
	echo `tput cup 29 23`"file name : $file"
	if [ "$2" = "d" ]; then
			echo `tput cup 30 23`[34m"file type : directory"; echo [0m
	elif [ "$2" = "-" ]; then
			echo `tput cup 30 23`"file type : normal file";
	elif [ "$2" = "x" ]; then
			echo `tput cup 30 23`[31m"file type : implement file"; echo [0m
	else
			echo `tput cup 30 23`[32m"file type : FIFO"; echo [0m
	fi
	echo `tput cup 31 23`"file size : `ls -dl $file | awk '{print $5}'`"
	echo `tput cup 32 23`"creation time : `ls --time-style full-iso -dl $file | cut -f 6,7,8 -d ' '`"
	echo `tput cup 33 23`"permission : `stat -c "%a" $file`"
	echo `tput cup 34 23`"absolute path : `ls -d $PWD/$file`"
}
ui()
{
		lay_out; left_ui; bottom_ui; middle_ui $1
}
rev_print()
{
	check=`echo "${arr[\`expr $vertical \* 5 + $horizontal\`]}" | cut -c1`; 
		
	if [ "$check" = "d" ]; then
		tput rev; directory_shape "${arr[`expr $vertical \* 5 + $horizontal`]}" $h $w; tput sgr0
		information "${arr[`expr $vertical \* 5 + $horizontal`]}" "d"
	elif [ "$check" = "-" ]; then
		check=`echo "${arr[\`expr $vertical \* 5 + $horizontal\`]}" | cut -c4`
		if [ "$check" = "x" ]; then
			tput rev; xfile_shape "${arr[`expr $vertical \* 5 + $horizontal`]}" $h $w; tput sgr0
			information "${arr[`expr $vertical \* 5 + $horizontal`]}" "x"
		else
			tput rev; file_shape "${arr[`expr $vertical \* 5 + $horizontal`]}" $h $w; tput sgr0
			information "${arr[`expr $vertical \* 5 + $horizontal`]}" "-"
		fi
	elif [ "$check" = "c" ]; then
		tput rev; upper_dir; tput sgr0
	else
		tput rev; sfile_shape "${arr[`expr $vertical \* 5 + $horizontal`]}" $h $w; tput sgr0
		information "${arr[`expr $vertical \* 5 + $horizontal`]}" "s"
	fi
}
declare -a copyarr
declare -a copyarr_path
declare -a movearr
declare -a movearr_path
while [ : ]
do
	ui
	
	if [ "`ls -l | awk '{print $2}'`" = "0" ]; then
		arr=("cd ..")
	else
		arr=("cd .." $(ls -dl * | grep ^d | awk '{print $1 $9}'; ls -dl * | grep ^- | awk '{print $1 $9}'; ls -dl * | grep -v ^d | grep -v ^- 	| awk '{print $1 $9}'))
	fi
		c=0; w=23; h=1;
		declare -i sum=`ls -l | wc -l`
		sum=`expr $sum - 1`
		scroll=0; index=0; horizontal=0; vertical=0
		if [ $horizontal = 0 ] && [ $vertical = 0 ]; then
			tput rev; upper_dir; tput sgr0
			w=23; h=1;
			tput cup 38 0
			while [ : ]
			do
				read -s -n 1 k
				if [ "$k" = "" ]; then
					cd ..
					continue 2
				fi
				if [ "$k" = "p" ]; then
					if [ "${copyarr[0]}" = "" ]; then
						echo "Got nothing to paste"
						continue
					fi
					for((i=0; i<3; i++))
					do
						if [ "${copyarr[$i]}" != "" ]; then
							var=`pwd`
							cp ${copyarr_path[$i]}/${copyarr[$i]} $var
							copyarr[$i]=""; copyarr_path[$i]=""
						fi
					done
					continue 2
				fi
				if [ "$k" = "v" ]; then
					if [ "${movearr[0]}" = "" ]; then
						echo "Got nothing to move"
						continue
					fi
					for((i=0; i<3; i++))
					do
						if [ "${movearr[$i]}" != "" ]; then
							var=`pwd`
							mv ${movearr_path[$i]}/${movearr[$i]} $var
							movearr[$i]=""; movearr_path[$i]=""
						fi
					done
					continue 2
				fi
				if [ "$k" = "" ]; then
						:
				else
						continue
				fi
				read -s -n 1 e
				read -s -n 1 y
				if [ "$y" = "A" ] || [ "$y" = "B" ] || [ "$y" = "D" ] || [ "$y" = "C" ] ; then
					break
				fi
			done
			c=`expr $c + 1`
		fi
		while [ : ]
		do
			if [ "$y" = "A" ]; then
			####위쪽 방향키
				if [ `expr $vertical \* 5 + $horizontal - 5` -lt 0 ]; then
					:
				else
					if [ $scroll -gt 0 ] && [ $c -lt 6 ]; then
						scroll=`expr $scroll - 1`
						vertical=`expr $vertical - 1`
						ui -5; rev_print
					else
						vertical=`expr $vertical - 1`; h=`expr $h - 5`; c=`expr $c - 5`
						ui; rev_print		
					fi
				fi
			elif [ "$y" = "B" ]; then
			###아래쪽 방향키
				if [ `expr $vertical \* 5 + $horizontal + 5` -gt $sum ]; then
					:
				else
					if [ $sum -gt 24 ] && [ $c -gt 20 ]; then
						scroll=`expr $scroll + 1`
						vertical=`expr $vertical + 1`
						ui 5; rev_print
					else
						vertical=`expr $vertical + 1`; h=`expr $h + 5`; c=`expr $c + 5`
						ui; rev_print
					fi
				fi
				if [ "$k" = "p" ]; then
					for((i=0; i<3; i++))
					do
						if [ "${copyarr[$i]}" != "" ]; then
							var=`pwd`
							cp ${copyarr_path[$i]}/${copyarr[$i]} $var
							copyarr[$i]=""; copyarr_path[$i]=""
							echo "paste!"
						fi
					done
					continue
				fi
			elif [ "$y" = "D" ]; then
			###왼쪽 방향키
				if [ `expr $vertical \* 5 + $horizontal` -eq 0 ]; then
					:
				else
					if [ $scroll -gt 0 ] && [ $c -eq 1 ]; then
						scroll=`expr $scroll - 1`
						horizontal=`expr $horizontal - 1`
						c=`expr $c + 4`
						w=79;  ui -5; rev_print
					else

						horizontal=`expr $horizontal - 1`; w=`expr $w - 14`			
	
						for((i=1; i<5; i++))
						do
							if [ $c -eq `expr 5 \* $i + 1` ]; then
								h=`expr $h - 5`
								w=79
							fi
						done
	
						c=`expr $c - 1`
						ui; rev_print
					fi
				fi
			else
			####오른쪽 방향키
				if [ "${arr[`expr $vertical \* 5 + $horizontal + 1`]}" = "" ]; then
					:
				else
					if [ $sum -gt 24 ] && [ $c -eq 25 ]; then
						scroll=`expr $scroll + 1`
						horizontal=`expr $horizontal + 1`
						w=23; c=`expr $c - 4`
						ui 5; rev_print
					else
						horizontal=`expr $horizontal + 1`; w=`expr $w + 14`
				
						for((i=1; i<5; i++))
						do
							if [ $c -eq `expr 5 \* $i` ]; then
								h=`expr $h + 5`; w=23
							fi
						done
					c=`expr $c + 1`
					ui; rev_print
					fi
				fi
			fi
			tput cup 38 0
			while [ : ]
			do
				read -s -n 1 k
				if [ "$k" = "" ]; then
					action=`echo "${arr[\`expr $vertical \* 5 + $horizontal\`]}" | cut -c1`
					fname=`echo "${arr[\`expr $vertical \* 5 + $horizontal\`]}" | cut -c11-`
					if [ "$action" = "d" ]; then
						index=0
						cd $fname/
						continue 3
					elif [ "$action" = "c" ]; then
						cd ..
						continue 3
					else
						xcheck=`echo "${arr[\`expr $vertical \* 5 + $horizontal\`]}" | cut -c4`
						if [ "$xcheck" = "x" ]; then
							./$fname
							continue 
						else
							continue
						fi
						
					fi
				fi
				if [ "$k" = "c" ]; then
					for((i=0; i<3; i++))
					do
						if [ "${copyarr[$i]}" = "" ]; then
							copyarr[$i]=`echo "${arr[\`expr $vertical \* 5 + $horizontal\`]}" | cut -c11-`
							copyarr_path[$i]=`pwd`
							echo "Save for copy"
							break
						fi
					done
					continue
				fi
				if [ "$k" = "m" ]; then
					for((i=0; i<3; i++))
					do
						if [ "${movearr[$i]}" = "" ]; then
							movearr[$i]=`echo "${arr[\`expr $vertical \* 5 + $horizontal\`]}" | cut -c11-`
							movearr_path[$i]=`pwd`
							echo "Save for move"
							break
						fi
					done
					continue 
				fi
				if [ "$k" = "p" ]; then
					if [ "${copyarr[0]}" = "" ]; then
						continue
					fi
					for((i=0; i<3; i++))
					do
						if [ "${copyarr[$i]}" != "" ]; then
							var=`pwd`
							cp ${copyarr_path[$i]}/${copyarr[$i]} $var
							copyarr[$i]=""; copyarr_path[$i]=""
						fi
					done
					continue 3
				fi
				if [ "$k" = "v" ]; then
					if [ "${movearr[0]}" = "" ]; then
						continue
					fi
					for((i=0; i<3; i++))
					do
						if [ "${movearr[$i]}" != "" ]; then
							var=`pwd`
							mv ${movearr_path[$i]}/${movearr[$i]} $var
							movearr[$i]=""; movearr_path[$i]=""
						fi
					done
					continue 3
				fi
				if [ "$k" = "r" ]; then
					rm -rf `echo "${arr[\`expr $vertical \* 5 + $horizontal\`]}" | cut -c11-`
					echo "File removed"; sleep 1
					continue 3
				fi
				read -s -n 1 e
				read -s -n 1 y
				if [ "$y" = "A" ] || [ "$y" = "B" ] || [ "$y" = "D" ] || [ "$y" = "C" ] ; then
					break
				fi
			done
		done
	
done
####커서 복귀
tput cup 38 0

