#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/file.h>
#include <tcl.h>

#define BUFSIZ 1024

int SystemCmd(ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[])
{
	Tcl_Obj *resultPtr;
	char buf[BUFSIZ];
	char *cmdstring, *p;
	FILE *pipe;
	int i, cmdlen, cmdlenavail;
	cmdlen = cmdlenavail = BUFSIZ;
	p = cmdstring = NULL;

	if(Tcl_PkgRequire(interp, "portui", "1.0", 0) == NULL) {
		return TCL_ERROR;
	}

	resultPtr = Tcl_GetObjResult(interp);

	if (objc < 2) {
		Tcl_WrongNumArgs(interp, 1, objv, "command");
		return TCL_ERROR;
	} else if (objc == 2) {
		cmdstring = Tcl_GetString(objv[1]);
	} else if (objc > 2) {
		cmdstring = malloc(cmdlen);
		if (cmdstring == NULL)
			return TCL_ERROR;
		p = cmdstring;
		/*
		 * Rather than realloc for every iteration
		 * through the argument vector, malloc a
		 * sizable chunk of memory first.
		 * If we extend beyond what is available,
		 * then realloc
		 */
		for (i = 1; i < objc; i++) {
			char *arg;
			int len;

			arg = Tcl_GetString(objv[i]);
			/* Add 1 for trailing \0 or ' ' */
			len = strlen(arg) + 1;

			if (len > cmdlenavail) {
				char *sptr;
				cmdlen += cmdlenavail + len;
				/*
				 * This is ugly because puma doesn't have
				 * reallocf. Remove when we rev past puma
				 * support
				 */
				sptr = cmdstring;
				cmdstring = realloc(cmdstring, cmdlen);
				if (cmdstring == NULL) {
					free(sptr);
					return TCL_ERROR;
				}
			}
			/* Subtract 1 to not copy trailing \0 */
			memcpy(p, arg, len - 1);
			p += len;

			if (i == objc - 1) {
				*(p - 1) = '\0';
			} else {
				*(p - 1) = ' ';
			}
			cmdlenavail -= len;
			cmdlen += len;
		}
	}

	pipe = popen(cmdstring, "r");
	if (p != NULL)
		free(cmdstring);

	while (fgets(buf, BUFSIZ, pipe) != NULL) {
		/* XXX We need the output but this is not at all correct */
		printf("%s", buf);
		Tcl_AppendToObj(resultPtr, (void *) &buf, strlen(buf));
	}

	switch (pclose(pipe)) {
		case 0:
			return TCL_OK;
		default:
			return TCL_ERROR;
	}
}

int FlockCmd(ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[])
{
	Tcl_Obj *resultPtr;
	const char errorstr[] = "use one of \"-shared\", \"-exclusive\", or \"-unlock\"";
	int operation = 0, fd, i;
	Tcl_Channel channel;
	ClientData handle;

	if (objc < 3 || objc > 4) {
		Tcl_WrongNumArgs(interp, 1, objv, "channelId switches");
		return TCL_ERROR;
	}

	resultPtr = Tcl_GetObjResult(interp);

    	if ((channel = Tcl_GetChannel(interp, Tcl_GetString(objv[1]), NULL)) == NULL)
		return TCL_ERROR;

	if (Tcl_GetChannelHandle(channel, TCL_READABLE|TCL_WRITABLE, &handle) != TCL_OK) {
		Tcl_SetStringObj(resultPtr, "error getting channel handle", -1);
		return TCL_ERROR;
	}
	fd = (int) handle;

	for (i = 2; i < objc; i++) {
		char *arg = Tcl_GetString(objv[i]);
		if (!strcmp(arg, "-shared")) {
			if (operation & LOCK_EX || operation & LOCK_UN) {
				Tcl_SetStringObj(resultPtr, (void *) &errorstr, -1);
				return TCL_ERROR;
			}
			operation |= LOCK_SH;
		} else if (!strcmp(arg, "-exclusive")) {
			if (operation & LOCK_SH || operation & LOCK_UN) {
				Tcl_SetStringObj(resultPtr, (void *) &errorstr, -1);
				return TCL_ERROR;
			}
			operation |= LOCK_EX;
		} else if (!strcmp(arg, "-unlock")) {
			if (operation & LOCK_SH || operation & LOCK_EX) {
				Tcl_SetStringObj(resultPtr, (void *) &errorstr, -1);
				return TCL_ERROR;
			}
			operation |= LOCK_UN;
		} else if (!strcmp(arg, "-noblock")) {
			if (operation & LOCK_UN) {
				Tcl_SetStringObj(resultPtr, "-noblock can not be used with -unlock", -1);
				return TCL_ERROR;
			}
			operation |= LOCK_NB;
		}
	}
	if (flock(fd, operation) != 0)
	{
		Tcl_SetStringObj(resultPtr, (void *) strerror(errno), -1);
		return TCL_ERROR;
	}
	return TCL_OK;
}

int Pextlib_Init(Tcl_Interp *interp)
{
	Tcl_CreateObjCommand(interp, "system", SystemCmd, NULL, NULL);
	Tcl_CreateObjCommand(interp, "flock", FlockCmd, NULL, NULL);
	if(Tcl_PkgProvide(interp, "Pextlib", "1.0") != TCL_OK)
		return TCL_ERROR;
	return TCL_OK;
}
