cmd_/home/hugh/UF_dev/assignment5/mycdrv.mod := printf '%s\n'   mycdrv.o | awk '!x[$$0]++ { print("/home/hugh/UF_dev/assignment5/"$$0) }' > /home/hugh/UF_dev/assignment5/mycdrv.mod
