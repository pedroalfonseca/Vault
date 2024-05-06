# MacPorts Installer addition on 2014-01-25_at_22:58:46: adding an appropriate PATH variable for use with MacPorts
if [ `uname` == "Darwin" ]; then
    export PATH=/opt/local/bin:/opt/local/sbin:$PATH
fi
# Finished adapting your PATH environment variable for use with MacPorts

# LS COLORS
if [ `uname` == "Linux" ]; then
    export LSCOLORS="ExGxBxDxCxEgEdxbxgxcxd"
elif [ `uname` == "Darwin" ]; then
    export LSCOLORS="ExGxBxDxCxEgEdxbxgxcxd"
    alias ls="ls -G"
fi

#export PS1="[\u@\h:\W] "
#export PS2=">"

# Fancy shell prompt with git status
# Adapted from https://gist.github.com/specious/8244801
parse_git_dirty () {
    [[ $(git status 2> /dev/null | tail -1) != "nothing to commit, working tree clean" ]] && echo "*"
}

parse_git_branch () {
    git branch --no-color 2> /dev/null | sed -e '/^[^*]/d' -e "s/* \(.*\)/\1$(parse_git_dirty)/"
}

show_git_prompt () {
    git branch 2>/dev/null 1>&2 && echo -e "($(parse_git_branch))"
}

if [[ -n $(type -t git) ]] ; then
    PS1="\$(show_git_prompt)"
else
    PS1=
fi

PS1="
\[\e[32;1m\]\u@\h\[\e[31;1m\] \[\e[34;1m\]\w\[\e[33;1m\] $PS1
\[\e[33;1m\] $ \[\e[0m\]"

# Display running command in GNU Screen window status
#
# In .screenrc, set: shelltitle "( |~"
#
# See: http://aperiodic.net/screen/title_examples#setting_the_title_to_the_name_of_the_running_program
case $TERM in screen*)
    PS1=${PS1}'\[\033k\033\\\]'
esac

# Locale and encoding
export LC_CTYPE=pt_BR.UTF-8

# Default browser
export BROWSER="firefox"

# Source aliases
if [ -f ~/.bash_aliases ]; then
    source ~/.bash_aliases
fi

# Set PATH so it includes user's private bin if it exists
if [ -d "$HOME/bin" ] ; then
    PATH="$HOME/bin:$PATH"
fi

# Some quotes
if [ -x "$(command -v fortune)" ]; then
    fortune
fi
