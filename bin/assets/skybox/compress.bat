mkdir processed
for %%F in (*.jpg) do (
    ffmpeg -i "%%F" -q:v 10 -vf scale=600:600 "compressed\%%F"
)