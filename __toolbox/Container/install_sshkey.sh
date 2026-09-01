#!/bin/sh

echo "Creating SSH keys for authentication on GitHUB"
echo ""

while true; do
  read -p "Enter your name: " USERNAME
  read -p "Enter your email: " EMAIL

  echo ""
  echo "You entered $USERNAME ($EMAIL)"
  echo ""

  read -p "Is this information correct? (yes/no): " choice
  case "$choice" in
        [Yy]*|[Yy][Ee][Ss])
            break
            ;;
	*)
	    echo ""
	    ;;
  esac
done

KEYDIR=/opt/.aluno
if [ ! -d "$KEYDIR" ]; then
    sudo mkdir -p $KEYDIR
    sudo chown aluno:aluno $KEYDIR
else
    rm $KEYDIR/*
fi
ssh-keygen -t ed25519 -C "$EMAIL" -f $KEYDIR/id_ed25519

cat << EOF > $KEYDIR/aluno.sh
SSHK=/opt/.aluno/id_ed25519
(ssh-add -L | grep $EMAIL) 2>&1 > /dev/null
if [ \$? -eq 1 ]; then
  echo "Setting up SSH ..."
  git config --global user.email "$EMAIL"
  git config --global user.name "$USERNAME"
  git config --global pull.rebase true
  ssh-add \$SSHK
fi

gsettings set org.gnome.desktop.wm.preferences button-layout 'appmenu:minimize,maximize,close'
gsettings set org.gnome.desktop.interface accent-color 'green'
gsettings set org.gnome.desktop.background picture-uri "file:///usr/share/backgrounds/gnome/adwaita-l.jxl"
dconf write /org/gnome/Ptyxis/bold-is-bright true
dconf write /org/gnome/Ptyxis/interface-style "'light'"
dconf write /org/gnome/Ptyxis/use-system-font false
dconf write /org/gnome/Ptyxis/font-name "'Monospace 18'"
EOF

sudo cp $KEYDIR/aluno.sh /etc/profile.d/

echo ""
echo "Please, install the above public key on your GitHUB account"
echo ""

cat $KEYDIR/id_ed25519.pub

