cmd_/home/hugh/UF_dev/assignment5/tuxdrv.mod := printf '%s\n'   tuxdrv.o | awk '!x[$$0]++ { print("/home/hugh/UF_dev/assignment5/"$$0) }' > /home/hugh/UF_dev/assignment5/tuxdrv.mod
