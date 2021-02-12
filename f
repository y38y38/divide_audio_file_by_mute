for mp3file in *.mp3; do
	ffmpeg -i "${mp3file}" -f s16le -acodec pcm_s16le "${mp3file%%.mp3}.raw"
done

for rawfile in *.raw; do
	./a.out "${rawfile}"
done
rm *.raw

for cutrawfile in *.cutraw; do
	ffmpeg -f s16le -ar 44.1k -ac 2  -i "${cutrawfile}" "${cutrawfile%%.cutraw}.wav"
done
rm *.cutraw

