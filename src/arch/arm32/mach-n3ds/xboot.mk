#
# Machine makefile
#

DEVKITPRO	?= /opt/devkitpro

#
# Sandbox namespace
#
NS_JMP		:=	-Dsetjmp=xboot_setjmp -Dlongjmp=xboot_longjmp

NS_CTYPE	:=	-Disalnum=xboot_isalnum -Disalpha=xboot_isalpha \
				-Disalpha=xboot_isalpha -Disascii=xboot_isascii \
				-Disblank=xboot_isblank -Discntrl=xboot_iscntrl \
				-Disdigit=xboot_isdigit -Disgraph=xboot_isgraph \
				-Dislower=xboot_islower -Disprint=xboot_isprint \
				-Dispunct=xboot_ispunct -Disspace=xboot_isspace \
				-Disupper=xboot_isupper -Disxdigit=xboot_isxdigit \
				-Dtoascii=xboot_toascii -Dtolower=xboot_tolower \
				-Dtoupper=xboot_toupper

NS_ENVIRON	:=	-Dclearenv=xboot_clearenv -Dgetenv=xboot_getenv \
				-Dputenv=xboot_putenv -Dsetenv=xboot_setenv \
				-Dunsetenv=xboot_unsetenv

NS_ERRNO	:=	-Dstrerror=xboot_strerror

NS_EXIT		:=	-Dabort=xboot_abort -Dexit=xboot_exit

NS_LOCALE	:=	-Dsetlocale=xboot_setlocale -Dlocaleconv=xboot_localeconv

NS_MALLOC	:=	-Dmalloc=xboot_malloc -Dmemalign=xboot_memalign \
				-Drealloc=xboot_realloc -Dcalloc=xboot_calloc \
				-Dfree=xboot_free

NS_STDIO	:=	-Dclearerr=xboot_clearerr -Dfclose=xboot_fclose \
				-Dfeof=xboot_feof -Dferror=xboot_ferror \
				-Dfflush=xboot_fflush -Dfgetc=xboot_fgetc \
				-Dfgetpos=xboot_fgetpos -Dfgets=xboot_fgets \
				-Dfopen=xboot_fopen -Dfprintf=xboot_fprintf \
				-Dfputc=xboot_fputc -Dfputs=xboot_fputs \
				-Dfread=xboot_fread -Dfreopen=xboot_freopen \
				-Dfscanf=xboot_fscanf -Dfseek=xboot_fseek \
				-Dfsetpos=xboot_fsetpos -Dftell=xboot_ftell \
				-Dfwrite=xboot_fwrite -Dgetc=xboot_getc \
				-Dgetchar=xboot_getchar -Dprintf=xboot_printf \
				-Dputchar=xboot_putchar -Dputc=xboot_putc \
				-Drewind=xboot_rewind -Dscanf=xboot_scanf \
				-Dsetbuf=xboot_setbuf -Dsetvbuf=xboot_setvbuf \
				-Dsnprintf=xboot_snprintf -Dsprintf=xboot_sprintf \
				-Dsscanf=xboot_sscanf -Dtmpfile=xboot_tmpfile \
				-Dtmpnam=xboot_tmpnam -Dungetc=xboot_ungetc \
				-Dvsnprintf=xboot_vsnprintf -Dvsscanf=xboot_vsscanf

NS_STDLIB	:=	-Drand=xboot_rand -Dsrand=xboot_srand \
				-Dabs=xboot_abs -Dlabs=xboot_labs \
				-Dllabs=xboot_llabs -Dllabs=xboot_llabs \
				-Datoi=xboot_atoi -Datol=xboot_atol \
				-Datoll=xboot_atoll -Datof=xboot_atof \
				-Dstrtol=xboot_strtol -Dstrtoll=xboot_strtoll \
				-Dstrtoul=xboot_strtoul -Dstrtoull=xboot_strtoull \
				-Dstrtod=xboot_strtod -Dstrtoimax=xboot_strtoimax \
				-Dstrtoumax=xboot_strtoumax -Dstrntoimax=xboot_strntoimax \
				-Dstrntoumax=xboot_strntoumax -Dbsearch=xboot_bsearch \
				-Ddiv=xboot_div -Dldiv=xboot_ldiv \
				-Dlldiv=xboot_lldiv -Dqsort=xboot_qsort

NS_STRING	:=	-Dstrcpy=xboot_strcpy -Dstrncpy=xboot_strncpy \
				-Dstrlcpy=xboot_strlcpy -Dstrcat=xboot_strcat \
				-Dstrncat=xboot_strncat -Dstrlcat=xboot_strlcat \
				-Dstrlen=xboot_strlen -Dstrnlen=xboot_strnlen \
				-Dstrcmp=xboot_strcmp -Dstrncmp=xboot_strncmp \
				-Dstrnicmp=xboot_strnicmp -Dstrcasecmp=xboot_strcasecmp \
				-Dstrncasecmp=xboot_strncasecmp -Dstrcoll=xboot_strcoll \
				-Dstrchr=xboot_strchr -Dstrrchr=xboot_strrchr \
				-Dstrnchr=xboot_strnchr -Dstrspn=xboot_strspn \
				-Dstrcspn=xboot_strcspn -Dstrpbrk=xboot_strpbrk \
				-Dstrstr=xboot_strstr -Dstrnstr=xboot_strnstr \
				-Dstrdup=xboot_strdup -Dstrsep=xboot_strsep \
				-Dmemset=xboot_memset -Dmemcpy=xboot_memcpy \
				-Dmemmove=xboot_memmove -Dmemchr=xboot_memchr \
				-Dmemscan=xboot_memscan -Dmemcmp=xboot_memcmp

NS_TIME		:=	-Dstrftime=xboot_strftime -Dasctime=xboot_asctime \
				-Dclock=xboot_clock -Dctime=xboot_ctime \
				-Ddifftime=xboot_difftime -Dgettimeofday=xboot_gettimeofday \
				-Dgmtime=xboot_gmtime -Dlocaltime=xboot_localtime \
				-Dtime=xboot_time

NS_MATH		:=	-Dacos=xboot_acos -Dacosf=xboot_acosf \
				-Dacosh=xboot_acosh -Dacoshf=xboot_acoshf \
				-Dasin=xboot_asin -Dasinf=xboot_asinf \
				-Dasinh=xboot_asinh -Dasinhf=xboot_asinhf \
				-Datan=xboot_atan -Datanf=xboot_atanf \
				-Datan2=xboot_atan2 -Datan2f=xboot_atan2f \
				-Datanh=xboot_atanh -Datanhf=xboot_atanhf \
				-Dcbrt=xboot_cbrt -Dcbrtf=xboot_cbrtf \
				-Dceil=xboot_ceil -Dceilf=xboot_ceilf \
				-Dcos=xboot_cos -Dcosf=xboot_cosf \
				-Dcosh=xboot_cosh -Dcoshf=xboot_coshf \
				-Dexp=xboot_exp -Dexpf=xboot_expf \
				-Dexp2=xboot_exp2 -Dexp2f=xboot_exp2f \
				-Dexpm1=xboot_expm1 -Dexpm1f=xboot_expm1f \
				-Dfabs=xboot_fabs -Dfabsf=xboot_fabsf \
				-Dfdim=xboot_fdim -Dfdimf=xboot_fdimf \
				-Dfloor=xboot_floor -Dfloorf=xboot_floorf \
				-Dfmod=xboot_fmod -Dfmodf=xboot_fmodf \
				-Dfrexp=xboot_frexp -Dfrexpf=xboot_frexpf \
				-Dhypot=xboot_hypot -Dhypotf=xboot_hypotf \
				-Dldexp=xboot_ldexp -Dldexpf=xboot_ldexpf \
				-Dlog=xboot_log -Dlogf=xboot_logf \
				-Dlog10=xboot_log10 -Dlog10f=xboot_log10f \
				-Dlog1p=xboot_log1p -Dlog1pf=xboot_log1pf \
				-Dlog2=xboot_log2 -Dlog2f=xboot_log2f \
				-Dmodf=xboot_modf -Dmodff=xboot_modff \
				-Dpow=xboot_pow -Dpowf=xboot_powf \
				-Drint=xboot_rint -Drintf=xboot_rintf \
				-Dround=xboot_round -Droundf=xboot_roundf \
				-Dscalbn=xboot_scalbn -Dscalbnf=xboot_scalbnf \
				-Dscalbln=xboot_scalbln -Dscalblnf=xboot_scalblnf \
				-Dsin=xboot_sin -Dsinf=xboot_sinf \
				-Dsinh=xboot_sinh -Dsinhf=xboot_sinhf \
				-Dsqrt=xboot_sqrt -Dsqrtf=xboot_sqrtf \
				-Dtan=xboot_tan -Dtanf=xboot_tanf \
				-Dtanh=xboot_tanh -Dtanhf=xboot_tanhf \
				-Dtrunc=xboot_trunc -Dtruncf=xboot_truncf \
				-D__cos=xboot___cos -D__cosdf=xboot___cosdf \
				-D__expo2=xboot___expo2 -D__expo2f=xboot___expo2f \
				-D__fpclassify=xboot___fpclassify -D__fpclassifyf=xboot___fpclassifyf \
				-D__rem_pio2_large=xboot___rem_pio2_large -D__rem_pio2=xboot___rem_pio2 \
				-D__rem_pio2f=xboot___rem_pio2f -D__sin=xboot___sin \
				-D__sindf=xboot___sindf -D__tan=xboot___tan \
				-D__tandf=xboot___tandf

NS_FILEIO	:=	-Dmount=xboot_mount -Dsync=xboot_sync \
				-Dumount=xboot_umount -Dopen=xboot_open \
				-Dread=xboot_read -Dwrite=xboot_write \
				-Dlseek=xboot_lseek -Dfstat=xboot_fstat \
				-Dioctl=xboot_ioctl -Dfsync=xboot_fsync \
				-Dclose=xboot_close -Dopendir=xboot_opendir \
				-Dreaddir=xboot_readdir -Drewinddir=xboot_rewinddir \
				-Dclosedir=xboot_closedir -Dgetcwd=xboot_getcwd \
				-Dchdir=xboot_chdir -Dmkdir=xboot_mkdir \
				-Drmdir=xboot_rmdir -Dstat=xboot_stat \
				-Daccess=xboot_access -Drename=xboot_rename \
				-Dunlink=xboot_unlink -Dremove=xboot_remove \
				-Dmknod=xboot_mknod -Dchmod=xboot_chmod \
				-Dchown=xboot_chown -Dumask=xboot_umask \
				-Dftruncate=xboot_ftruncate -Dtruncate=xboot_truncate \
				-Dreadv=xboot_readv -Dwritev=xboot_writev

NS_TEMP		:=	-Dmktime=xboot_mktime -Dctrlc=xboot_ctrlc \
				-Dsystem=xboot_system -Dparser=xboot_parser \
				-Dreadline=xboot_readline

DEFINES		+=	$(NS_JMP) $(NS_CTYPE) $(NS_ENVIRON) $(NS_ERRNO) \
				$(NS_EXIT) $(NS_LOCALE) $(NS_MALLOC) \
				$(NS_STDIO) $(NS_STDLIB) $(NS_STRING) $(NS_TIME) \
				$(NS_MATH) $(NS_FILEIO) $(NS_TEMP)

DEFINES		+=	-D__ARM32_ARCH__=6 -D__SANDBOX__

ASFLAGS		:= -g -ggdb -Wall -O3
CFLAGS		:= -g -ggdb -Wall -O3
CXXFLAGS	:= -g -ggdb -Wall -O3 -fno-rtti -fno-exceptions
MCFLAGS		:= -march=armv6k -mtune=mpcore -mfloat-abi=hard -mtp=soft -mword-relocations -fomit-frame-pointer -ffunction-sections
LDFLAGS		:= -specs=arch/$(ARCH)/$(MACH)/xboot.specs $(MCFLAGS)

LIBDIRS		:= arch/$(ARCH)/$(MACH)/libsandbox $(DEVKITPRO)/libctru/lib
LIBS 		:= -lsandbox -lctru -lm
INCDIRS		:= arch/$(ARCH)/$(MACH)/libsandbox
SRCDIRS		:=

ifeq ($(strip $(HOSTOS)), linux)
3DSXTOOL	:= arch/$(ARCH)/$(MACH)/tools/linux/3dsxtool
MAKEROM		:= arch/$(ARCH)/$(MACH)/tools/linux/makerom
endif
ifeq ($(strip $(HOSTOS)), windows)
3DSXTOOL	:= arch/$(ARCH)/$(MACH)/tools/windows/3dsxtool
MAKEROM		:= arch/$(ARCH)/$(MACH)/tools/windows/makerom
endif
SMDH		:= arch/$(ARCH)/$(MACH)/tools/images/xboot.smdh

xbegin:
	@echo Building libsandbox
	@$(MAKE) DEVKITPRO="$(DEVKITPRO)" -C arch/$(ARCH)/$(MACH)/libsandbox

xend:
	@echo Building $(X_NAME).3dsx
	@$(3DSXTOOL) $(X_NAME) $(X_NAME).3dsx --smdh=$(SMDH)

xclean:
	@echo Clean libsandbox
	@$(MAKE) DEVKITPRO="$(DEVKITPRO)" -C arch/$(ARCH)/$(MACH)/libsandbox clean
