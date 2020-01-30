. ./build_variables.ps1

MMT_REPO_URL="https://github.com/Zackptg5/MMT-Extended/archive/master.zip"
MMT_REPO_ZIP_OUTPUT="$RepoLibsDir/MMT-Ex-master.zip"
MMT_ZIP_FOLDER_NAME = "MMT-Extended-master"

Start-BitsTransfer -Source $MMT_REPO_URL -Destination $MMT_REPO_ZIP_OUTPUT

# Extract Zip File to ./libs/MMT-Ex/
