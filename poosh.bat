MESSAGE=":)"
if [ "$#" -ne 0 ] && [ -n "$1" ]; then
 MESSAGE="$*"
fi

git config --global http.postBuffer 524288000
git pull & git add --all && git commit -m "$MESSAGE" && git push
