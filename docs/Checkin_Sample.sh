# This bash file changes identity, commits ALL changes, and pushes.
# Be sure to set 'name', 'email', and 'username'
NAME="Robert Wolff"
EMAIL="bob.wolff68@gmail.com"
GITUSERNAME="bobwolff68"

git config user.name $NAME
git config user.email $EMAIL
git commit -a
git push https://$GITUSERNAME@github.com/bobwolff68/FRCTeam1967
