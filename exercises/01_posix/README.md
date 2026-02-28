# POSIX Exercise

## 1. Install a POSIX system on your device (box)

As I am using a Windows machine, I followed the option of installing a POSIX-compatible environment using Windows Subsystem for Linux (WSL).

Steps followed:

- Installed WSL from Administrator PowerShell following Microsoft's official guide.
- Chose Ubuntu 24.04 LTS as the POSIX environment.
- Verified terminal access using `wsl` from Administrator PowerShell.

## 2. Learn how to start a terminal in your POSIX system

As I am using Ubuntu via WSL on a Windows machine, there are two options, either

- Open the Windows Start Menu -> search for "Ubuntu" -> launch it 

or

- Open the Windows Start Menu -> search for "PowerShell" -> open it as administrator --> type: wsl

Once the terminal opens, I can optionally verify my environment:

- Check current directory: `pwd`
- Check current user: `whoami`
- List files: `ls`
- Navigate directories: `cd <directory>`

## 3. Read about the man utility. Run man man and read the description of the utility in your system.

- The `man` command displays the **manual page** for other commands in my POSIX system. It is a built-in way to read documentation directly from the terminal.
- It can be used to read manuals for any installed command by typing, for instance:
	- `man ls`
	- `man cp`
	- `man mkdir`
- To read the manual for `man` itself, type:
	- `man man`
- The manual will open using the default pager (`less`). Scrolling is done with the **arrow keys**.
- To search inside the manual, press `/` followed by the search term and Enter.
- To quit the manual and return to the shell, press `q`.

## 4. Learn the following commands (file utilities) from the GNU Core Utilities: cp, ls, mkdir, mv, rm, and the bash built-in command cd.

The following table summarizes the basic purpose of each command mentioned.

| Command | Description |
|---------|-------------|
| `ls`   | List directory contents |
| `cp`   | Copy files or directories |
| `mv`   | Move or rename files/directories |
| `rm`   | Remove files or directories |
| `mkdir`| Create new directories |
| `cd`   | Change current working directory |


## 5. Install bash-completion with the command "sudo apt install bash-completion". Read about the command-line completion.

- Bash-completion was installed as described.
- Bash-completion enhances the shell with **command-line completion**, which allows the user to type only the first few characters of an item (command, program or filename) and have the remaining part filled by pressing `Tab`.

Examples:
- Type `cd /h` and press Tab -> may complete to `/home/`
- Type `ls --v` and press Tab -> may complete to `ls --version`

## 6. Install your favourite text editor.

I chose Visual Studio Code as my text editor, as it is already installed on my system.

## 7. Learn to create, save and edit text files with your favourite text editor.

In my setup (Ubuntu via WSL) and editor (VS Code), the workflow is as follows.

### Step-by-step workflow to create and edit a text file in VSC and save it in my Linux directory via the WSL terminal

1. Launch PowerShell as Administrator and type the following.
	```bash
	wsl			# launches the WSL terminal
	cd ~		# redirects to my Linux home directory
	pwd			# prints working directory (should be "/home/thorbj" in my case)
	code . 		# launches VSC from the currenct working directory
2. In VSC, create, edit and save a new text file.
	- Click: File $\rightarrow$ New File $\rightarrow$ Text File
	- Press: `Ctrl` + `S`
	- Enter a filename: `<FILENAME>`.txt
	- Press: `Enter`.
	- The file is now stored in the working directory printed by `pwd` in step 1
	- Edit the file as needed
	- Save changes by pressing `Ctrl` + `S`
3. Back in the WSL terminal, verify the text file created in previous step.
	```bash
	ls     				# lists files in the directory
	cat	<FILENAME>.txt	# displays file content
4. Verify the output of previous step.
	- The file should appear in the directory listing, and `cat` should print the file contents.

At this point, the text file `<FILENAME>`.txt has been successfully created, edited, saved, and verified using both the text editor and standard POSIX commands.

