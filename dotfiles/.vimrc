syntax on
set encoding=utf-8
set tabstop=4
set shiftwidth=4
set expandtab
set autoindent
set number
set relativenumber
set cursorline
set hlsearch
set incsearch
set ignorecase
set smartcase
set list
set listchars=trail:·,tab:▸·,nbsp:␣
set completeopt=menu,menuone,longest
set termguicolors

inoremap <expr> <tab> pumvisible() ? '<C-y>' : '<tab>'

let g:netrw_banner = 0

call plug#begin()
Plug 'vim-scripts/AutoComplPop'
Plug 'jiangmiao/auto-pairs'
Plug 'junegunn/fzf.vim'
Plug 'junegunn/fzf', { 'do': { -> fzf#install() } }
Plug 'dense-analysis/ale'
Plug 'nanotech/jellybeans.vim'
Plug 'vim-airline/vim-airline'
Plug 'vim-airline/vim-airline-themes'
call plug#end()

let mapleader = ' '

nnoremap <silent> <leader>n :noh<CR>

nnoremap <silent> <leader>f :Files<CR>
nnoremap <silent> <leader>pf :Files %:p:h<CR>

nnoremap <silent> <leader>t :tabnew<CR>
nnoremap <silent> <leader>l :tabnext<CR>
nnoremap <silent> <leader>h :tabprevious<CR>
nnoremap <silent> <leader>1 1gt
nnoremap <silent> <leader>2 2gt
nnoremap <silent> <leader>3 3gt
nnoremap <silent> <leader>4 4gt
nnoremap <silent> <leader>5 5gt
nnoremap <silent> <leader>6 6gt
nnoremap <silent> <leader>7 7gt
nnoremap <silent> <leader>8 8gt
nnoremap <silent> <leader>9 9gt
nnoremap <silent> <leader>0 :tablast<CR>

nnoremap <silent> <C-b> :enew<CR>
nnoremap <silent> <C-k> :bdelete<CR>
nnoremap <silent> <C-l> :bnext<CR>
nnoremap <silent> <C-h> :bprevious<CR>

let b:ale_linters = {
\  'c': ['gcc'],
\  'cpp': ['g++'],
\  'python': ['flake8'],
\}

colorscheme jellybeans

let g:airline#extensions#tabline#enabled = 1
let g:airline#extensions#tabline#tab_nr_type = 1

let g:airline_theme="jellybeans"
