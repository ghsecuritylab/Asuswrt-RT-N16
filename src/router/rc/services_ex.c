/*
 * Copyright 2004, ASUSTek Inc.
 * All Rights Reserved.
 * 
 * THIS SOFTWARE IS OFFERED "AS IS", AND ASUS GRANTS NO WARRANTIES OF ANY
 * KIND, EXPRESS OR IMPLIED, BY STATUTE, COMMUNICATION OR OTHERWISE. BROADCOM
 * SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A SPECIFIC PURPOSE OR NONINFRINGEMENT CONCERNING THIS SOFTWARE.
 *
 * $Id: services_ex.c,v 1.24 2008/11/10 02:30:39 james26_jang Exp $
 */

#ifdef ASUS_EXT
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <errno.h>
#include <sys/fcntl.h>
#include <dirent.h>
#include <sys/mount.h>
#include <bcmnvram.h>
#include <netconf.h>
#include <shutils.h>
#include <rc.h>
#include <syslog.h>
#include "iboxcom.h"
#include "lp.h"
#include <sys/vfs.h>
#include <sys/types.h>
#include <sys/stat.h>

#ifdef WCN
#include "ILibParsers.h"
#include <wlutils.h>
#endif

#ifdef DLM
#include <asm/page.h>
#include <sys/swap.h>
#include <sys/sysinfo.h>
#include <stdarg.h>	// 2008.02 James.
#endif

#include <shutils.h>
#include <security_ipc.h>
#include <wlif_utils.h>


#define logs(s) syslog(LOG_NOTICE, s)

#ifdef USB_SUPPORT
#define PRINTER_SUPPORT 1
#define MASSSTORAGE_SUPPORT 1
#ifdef DLM
#define AUDIO_SUPPORT 1
#define WEBCAM_SUPPORT 1
#endif

//#define PARPORT_SUPPORT 
//#define USBCOPY_SUPPORT 1


#ifdef DLM
/* CRC lookup table */
static unsigned long crcs[256]={ 0x00000000,0x77073096,0xEE0E612C,0x990951BA,
0x076DC419,0x706AF48F,0xE963A535,0x9E6495A3,0x0EDB8832,0x79DCB8A4,0xE0D5E91E,
0x97D2D988,0x09B64C2B,0x7EB17CBD,0xE7B82D07,0x90BF1D91,0x1DB71064,0x6AB020F2,
0xF3B97148,0x84BE41DE,0x1ADAD47D,0x6DDDE4EB,0xF4D4B551,0x83D385C7,0x136C9856,
0x646BA8C0,0xFD62F97A,0x8A65C9EC,0x14015C4F,0x63066CD9,0xFA0F3D63,0x8D080DF5,
0x3B6E20C8,0x4C69105E,0xD56041E4,0xA2677172,0x3C03E4D1,0x4B04D447,0xD20D85FD,
0xA50AB56B,0x35B5A8FA,0x42B2986C,0xDBBBC9D6,0xACBCF940,0x32D86CE3,0x45DF5C75,
0xDCD60DCF,0xABD13D59,0x26D930AC,0x51DE003A,0xC8D75180,0xBFD06116,0x21B4F4B5,
0x56B3C423,0xCFBA9599,0xB8BDA50F,0x2802B89E,0x5F058808,0xC60CD9B2,0xB10BE924,
0x2F6F7C87,0x58684C11,0xC1611DAB,0xB6662D3D,0x76DC4190,0x01DB7106,0x98D220BC,
0xEFD5102A,0x71B18589,0x06B6B51F,0x9FBFE4A5,0xE8B8D433,0x7807C9A2,0x0F00F934,
0x9609A88E,0xE10E9818,0x7F6A0DBB,0x086D3D2D,0x91646C97,0xE6635C01,0x6B6B51F4,
0x1C6C6162,0x856530D8,0xF262004E,0x6C0695ED,0x1B01A57B,0x8208F4C1,0xF50FC457,
0x65B0D9C6,0x12B7E950,0x8BBEB8EA,0xFCB9887C,0x62DD1DDF,0x15DA2D49,0x8CD37CF3,
0xFBD44C65,0x4DB26158,0x3AB551CE,0xA3BC0074,0xD4BB30E2,0x4ADFA541,0x3DD895D7,
0xA4D1C46D,0xD3D6F4FB,0x4369E96A,0x346ED9FC,0xAD678846,0xDA60B8D0,0x44042D73,
0x33031DE5,0xAA0A4C5F,0xDD0D7CC9,0x5005713C,0x270241AA,0xBE0B1010,0xC90C2086,
0x5768B525,0x206F85B3,0xB966D409,0xCE61E49F,0x5EDEF90E,0x29D9C998,0xB0D09822,
0xC7D7A8B4,0x59B33D17,0x2EB40D81,0xB7BD5C3B,0xC0BA6CAD,0xEDB88320,0x9ABFB3B6,
0x03B6E20C,0x74B1D29A,0xEAD54739,0x9DD277AF,0x04DB2615,0x73DC1683,0xE3630B12,
0x94643B84,0x0D6D6A3E,0x7A6A5AA8,0xE40ECF0B,0x9309FF9D,0x0A00AE27,0x7D079EB1,
0xF00F9344,0x8708A3D2,0x1E01F268,0x6906C2FE,0xF762575D,0x806567CB,0x196C3671,
0x6E6B06E7,0xFED41B76,0x89D32BE0,0x10DA7A5A,0x67DD4ACC,0xF9B9DF6F,0x8EBEEFF9,
0x17B7BE43,0x60B08ED5,0xD6D6A3E8,0xA1D1937E,0x38D8C2C4,0x4FDFF252,0xD1BB67F1,
0xA6BC5767,0x3FB506DD,0x48B2364B,0xD80D2BDA,0xAF0A1B4C,0x36034AF6,0x41047A60,
0xDF60EFC3,0xA867DF55,0x316E8EEF,0x4669BE79,0xCB61B38C,0xBC66831A,0x256FD2A0,
0x5268E236,0xCC0C7795,0xBB0B4703,0x220216B9,0x5505262F,0xC5BA3BBE,0xB2BD0B28,
0x2BB45A92,0x5CB36A04,0xC2D7FFA7,0xB5D0CF31,0x2CD99E8B,0x5BDEAE1D,0x9B64C2B0,
0xEC63F226,0x756AA39C,0x026D930A,0x9C0906A9,0xEB0E363F,0x72076785,0x05005713,
0x95BF4A82,0xE2B87A14,0x7BB12BAE,0x0CB61B38,0x92D28E9B,0xE5D5BE0D,0x7CDCEFB7,
0x0BDBDF21,0x86D3D2D4,0xF1D4E242,0x68DDB3F8,0x1FDA836E,0x81BE16CD,0xF6B9265B,
0x6FB077E1,0x18B74777,0x88085AE6,0xFF0F6A70,0x66063BCA,0x11010B5C,0x8F659EFF,
0xF862AE69,0x616BFFD3,0x166CCF45,0xA00AE278,0xD70DD2EE,0x4E048354,0x3903B3C2,
0xA7672661,0xD06016F7,0x4969474D,0x3E6E77DB,0xAED16A4A,0xD9D65ADC,0x40DF0B66,
0x37D83BF0,0xA9BCAE53,0xDEBB9EC5,0x47B2CF7F,0x30B5FFE9,0xBDBDF21C,0xCABAC28A,
0x53B39330,0x24B4A3A6,0xBAD03605,0xCDD70693,0x54DE5729,0x23D967BF,0xB3667A2E,
0xC4614AB8,0x5D681B02,0x2A6F2B94,0xB40BBE37,0xC30C8EA1,0x5A05DF1B,0x2D02EF8D};
#endif

enum
{
	WEB_NONE = 0,
	WEB_PWCWEB,
	WEB_OVWEB,
	WEB_AUDIO,
	USB_PRINTER, //2008.05 James.
	MASS_STORAGE //2008.03.13 Yau add
} WEBTYPE;

char *PWCLIST[] = {"471","69a","46d","55d","41e","4cc","d81", NULL};
char *OVLIST[] = {"5a9","813","b62", NULL};

#endif

/* WPS control functions */
static int write_to_wps(int fd, char *cmd);
static int read_from_wps(int fd, char *databuf, int datalen);
int parse_wps_env(char *buf);
static int get_wps_env();
static int set_wps_env(char *uibuf);

static int wps_config_command;
static int wps_action;
static char wps_uuid[36];
static char wps_unit[32];

#define USLEEP(X)       usleep(X)


char buf_g[512];
char usb1[128];
char usb2[128];

void diag_PaN(void)
{
	FILE *fp;
//	char *token;
//    	char                                mfr[32];
//    	char                                model[64];
//    	int                                 fd;
//	int  i = 0;
        
//    	struct parport_splink_device_info   prn_info;

	/* dump pci device */
	fp=fopen("/proc/pci", "r");
	if (fp!=NULL)
	{
		while(fgets(buf_g, sizeof(buf_g), fp))
		{
			if(strstr(buf_g, "PCI device"))
				fprintf(stderr, buf_g);
		}
		fclose(fp);
	}

#ifdef PRINTER_SUPPORT
#ifdef PARPORT_SUPPORT	
    if( (fd=open("/dev/lp0", O_RDWR)) < 0 ) //Someone is opening the lp0
    {
        fp=fopen("/proc/sys/dev/parport/parport0/devices/lp/deviceid","r");

        if( fp != NULL)
        {    
            while ( fgets(buf_g, sizeof(buf_g), fp) != NULL )  
            {
                if(buf_g[0] == '\n')
                {
                    continue;
                }
                if(strncmp(buf_g , "Model: " , strlen("Model: ")) == 0)
                {
                    token = buf_g + strlen("Model: ");
		    sprintf(buf_g, "PARPORT: %s", token);
		    fprintf(stderr, "%s\r\n", buf_g);
		    //logmessage("PARPORT", buf_g);
                }                
	    }	
	    fclose(fp);
	}
	//else logmessage("NO", "No printer found");
    }
    else
    {
        if( ioctl(fd, LPGETID, &prn_info) <0 )
        {
	    //logmessage("PaN", "No printer found");
            //PRINT("ioctl error\n");
        }
        else
        {
            memccpy(mfr , prn_info.mfr , 1 , 32);
            memccpy(model , prn_info.model , 1 , 32);
        
            sprintf(buf_g, "PARPORT: %s", model);
	    fprintf(stderr, "%s\r\n", buf_g);
            //logmessage("PARPORT", buf_g);
        }
	close(fd);
   }
#endif
#endif
   fprintf(stderr, "echo for PaN ::: &&&PaN\r\n");
}

int
start_dhcpd(void)
{
	FILE *fp;
	char *dhcpd_argv[] = {"udhcpd", "/tmp/udhcpd.conf", NULL, NULL};
	char *slease = "/tmp/udhcpd-br0.sleases";
	pid_t pid;
	int dns_count=0;

#ifdef PBCENR
	if (	nvram_match("sw_mode_ex", "3") ||               // Jiahao for RT-N12
	       (nvram_match("sw_mode_ex", "1") && nvram_invmatch("lan_proto", "dhcp")))
        {
                fprintf(stderr, "skip running...\n");
                return 0;
        }
        else
                fprintf(stderr, "starting...\n");
#else
	if (nvram_match("router_disable", "1") || nvram_invmatch("lan_proto", "dhcp"))
		return 0;
#endif

	dprintf("%s %s %s %s\n",
		nvram_safe_get("lan_ifname"),
		nvram_safe_get("dhcp_start"),
		nvram_safe_get("dhcp_end"),
		nvram_safe_get("lan_lease"));

	if (!(fp = fopen("/tmp/udhcpd-br0.leases", "a"))) {
		perror("/tmp/udhcpd-br0.leases");
		return errno;
	}
	fclose(fp);

	/* Write configuration file based on current information */
	if (!(fp = fopen("/tmp/udhcpd.conf", "w"))) {
		perror("/tmp/udhcpd.conf");
		return errno;
	}
	
	fprintf(fp, "pidfile /var/run/udhcpd-br0.pid\n");
	fprintf(fp, "start %s\n", nvram_safe_get("dhcp_start"));
	fprintf(fp, "end %s\n", nvram_safe_get("dhcp_end"));
	fprintf(fp, "interface %s\n", nvram_safe_get("lan_ifname"));
	fprintf(fp, "remaining yes\n");
	fprintf(fp, "lease_file /tmp/udhcpd-br0.leases\n");
	fprintf(fp, "option subnet %s\n", nvram_safe_get("lan_netmask"));
	
	if (nvram_invmatch("dhcp_gateway_x",""))
	    fprintf(fp, "option router %s\n", nvram_safe_get("dhcp_gateway_x"));	
	else	
	    fprintf(fp, "option router %s\n", nvram_safe_get("lan_ipaddr"));	

        /* dns patch check 0524 */
        dns_count = 0;

        fprintf(fp, "option dns %s\n", nvram_safe_get("lan_ipaddr"));
        ++dns_count;

        if (nvram_invmatch("dhcp_dns1_x",""))
        {
                fprintf(fp, "option dns %s\n", nvram_safe_get("dhcp_dns1_x"));
                ++dns_count;
        }

        if(dns_count < 2)
        {
                if (nvram_invmatch("auto_dhcp_dns","") && (strcmp(nvram_safe_get("dhcp_dns1_x"), nvram_safe_get("auto_dhcp_dns"))!=0))
                {
                        printf("[dhcpd] get extra option dns\n");
                        fprintf(fp, "option dns %s\n", nvram_safe_get("auto_dhcp_dns"));
                }
                else
                {
                        fprintf(fp, "option dns 8.8.8.8\n");
                        printf("[dhcpd] force set extra option dns\n");
                }
        }
        /* dns patch check 0524 end */
	
//	if (nvram_invmatch("dhcp_dns1_x",""))		
//		fprintf(fp, "option dns %s\n", nvram_safe_get("dhcp_dns1_x"));		
//	fprintf(fp, "option dns %s\n", nvram_safe_get("lan_ipaddr"));
	fprintf(fp, "option lease %s\n", nvram_safe_get("dhcp_lease"));

	if (nvram_invmatch("dhcp_wins_x",""))		
		fprintf(fp, "option wins %s\n", nvram_safe_get("dhcp_wins_x"));		
	if (nvram_invmatch("lan_domain", ""))
		fprintf(fp, "option domain %s\n", nvram_safe_get("lan_domain"));
	fclose(fp);

	
	if (nvram_match("dhcp_static_x","1"))
	{	
		write_static_leases(slease);
		dhcpd_argv[2] = slease;
	}
	else
	{
		dhcpd_argv[2] = NULL;
	}

	_eval(dhcpd_argv, NULL, 0, &pid);


	dprintf("done\n");
	return 0;
}

int
stop_dhcpd(void)
{
//	char sigusr1[] = "-XX";
	int ret;

/*
* Process udhcpd handles two signals - SIGTERM and SIGUSR1
*
*  - SIGUSR1 saves all leases in /tmp/udhcpd.leases
*  - SIGTERM causes the process to be killed
*
* The SIGUSR1+SIGTERM behavior is what we like so that all current client
* leases will be honorred when the dhcpd restarts and all clients can extend
* their leases and continue their current IP addresses. Otherwise clients
* would get NAK'd when they try to extend/rebind their leases and they 
* would have to release current IP and to request a new one which causes 
* a no-IP gap in between.
*/
	ret = eval("killall", "udhcpd");

	dprintf("done\n");
	return ret;
}

// dns patch check 0524 start
int
restart_dhcpd()
{
        printf("restart dhcpd\n");      /*/tmp test */

        stop_dhcpd();
        sleep(1);
        start_dhcpd();
        return 0;
}
// dns patch check 0524 end


int
start_dns(void)
{
	FILE *fp;
	char *dproxy_argv[] = {"dproxy", "-d", "-c", "/tmp/dproxy.conf", NULL};
	pid_t pid;
	char word[100];
	int ret, active;

	if (nvram_match("router_disable", "1"))
		return 0;


	/* Create resolv.conf with empty nameserver list */
	if (!(fp = fopen("/tmp/resolv.conf", "r")))
	{
		if (!(fp = fopen("/tmp/resolv.conf", "w"))) 
		{
			perror("/tmp/resolv.conf");
			return errno;
		}
		else fclose(fp);
	}
	else fclose(fp);

	if (!(fp = fopen("/tmp/dproxy.conf", "w"))) {
		perror("/tmp/dproxy.conf");
		return errno;
	}
	
	fprintf(fp, "name_server=140.113.1.1\n");
	fprintf(fp, "ppp_detect=no\n");
	fprintf(fp, "purge_time=1200\n");
	fprintf(fp, "deny_file=/tmp/dproxy.deny\n");
	fprintf(fp, "cache_file=/tmp/dproxy.cache\n");
	fprintf(fp, "hosts_file=/tmp/hosts\n");
	fprintf(fp, "dhcp_lease_file=\n");
	fprintf(fp, "ppp_dev=/var/run/ppp0.pid\n");
	fclose(fp);

	// if user want to set dns server by himself
	if (nvram_invmatch("wan_dnsenable_x", "1"))	
	{
		/* Write resolv.conf with upstream nameservers */
		if (!(fp = fopen("/tmp/resolv.conf", "w"))) {
			perror("/tmp/resolv.conf");
			return errno;
		}
	
		if (nvram_invmatch("wan_dns1_x",""))
			fprintf(fp, "nameserver %s\n", nvram_safe_get("wan_dns1_x"));		
		if (nvram_invmatch("wan_dns2_x",""))
			fprintf(fp, "nameserver %s\n", nvram_safe_get("wan_dns2_x"));
		fclose(fp);
	}

	active = timecheck_item(nvram_safe_get("url_date_x"), 
				nvram_safe_get("url_time_x"));

	if (nvram_match("url_enable_x", "1") && active) 
	{
		int i;
		
		if (!(fp = fopen("/tmp/dproxy.deny", "w"))) {
			perror("/tmp/dproxy.deny");
			return errno;
		}

		for(i=0; i<atoi(nvram_safe_get("url_num_x")); i++) {
			sprintf(word, "url_keyword_x%d", i);
			fprintf(fp, "%s\n", nvram_safe_get(word));
		}
	
		fclose(fp);	
	}
	else unlink("/tmp/dproxy.deny");


	if (!(fp = fopen("/tmp/hosts", "w"))) {
		perror("/tmp/hosts");
		return errno;
	}

	fprintf(fp, "127.0.0.1 localhost.localdomain localhost\n");
	fprintf(fp, "%s	my.router\n", nvram_safe_get("lan_ipaddr"));
	fprintf(fp, "%s	my.%s\n", nvram_safe_get("lan_ipaddr"), nvram_safe_get("productid"));
	
	if (nvram_invmatch("lan_hostname", ""))
	{
		fprintf(fp, "%s %s.%s %s\n", nvram_safe_get("lan_ipaddr"),
					nvram_safe_get("lan_hostname"),
					nvram_safe_get("lan_domain"),
					nvram_safe_get("lan_hostname"));
	}	
	fclose(fp);	
		
	_eval(dproxy_argv, NULL, 0, &pid);

	dprintf("done\n");

	return ret;
}	

int
stop_dns(void)
{
	int ret = eval("killall", "dproxy");

	unlink("/tmp/dproxy.deny");		
	dprintf("done\n");
	return ret;
}

int
ddns_updated_main(int argc, char *argv[])
{
	FILE *fp;
	char buf[64], *ip;

	if (!(fp=fopen("/tmp/ddns.cache", "r"))) return 0;
	
	fgets(buf, sizeof(buf), fp);
	fclose(fp);

	if (!(ip=strchr(buf, ','))) return 0;
	
	nvram_set("ddns_cache", buf);
	nvram_set("ddns_ipaddr", ip+1);
	nvram_set("ddns_status", "1");
	nvram_commit();

	logmessage("ddns", "ddns update ok");

	dprintf("done\n");

	return 0;
}
	

int 
start_ddns(void)
{
	FILE *fp;
//	char buf[64];
	char *wan_ip, *ddns_cache;
	char server[32];
	char user[32];
	char passwd[32];
	char host[64];
	char service[32];
	char usrstr[64];
	char wan_ifname[16];
	int  wild=nvram_match("ddns_wildcard_x", "1");
	char *dproxy_argv[] = {"dproxy", "-d", "-c", "/tmp/dproxy.conf", NULL};
	pid_t pid;


	if (nvram_match("router_disable", "1")) return -1;
	
	if (nvram_invmatch("ddns_enable_x", "1")) return -1;

	if ((wan_ip = nvram_safe_get("wan_ipaddr_t"))==NULL) return -1;

	if (nvram_match("ddns_ipaddr", wan_ip)) return -1;

	if (inet_addr(wan_ip)==inet_addr(nvram_safe_get("ddns_ipaddr"))) return -1;

	// TODO : Check /tmp/ddns.cache to see current IP in DDNS
	// update when,
	// 	1. if ipaddr!= ipaddr in cache
	// 	
        // update
	// * nvram ddns_cache, the same with /tmp/ddns.cache

	if ((fp=fopen("/tmp/ddns.cache", "r"))==NULL && 
	     (ddns_cache=nvram_get("ddns_cache"))!=NULL)
	{
		if ((fp = fopen("/tmp/ddns.cache", "w+"))!=NULL)
		{
			fprintf(fp, "%s", ddns_cache);
			fclose(fp);
		}
	}

	/* Clean DNS cache */
	//eval("rm", "-rf", "/tmp/dproxy.cache");
	unlink("/tmp/dproxy.cache");	// 2008.08 James.
//	eval("dproxy", "-d", "-c", "/tmp/dproxy.conf");
//	_eval(dproxy_argv, NULL, 0, &pid);

	strcpy(server, nvram_safe_get("ddns_server_x"));
	strcpy(user, nvram_safe_get("ddns_username_x"));
	strcpy(passwd, nvram_safe_get("ddns_passwd_x"));
	strcpy(host, nvram_safe_get("ddns_hostname_x"));
	strcpy(service, "");
			
	if (strcmp(server, "WWW.DYNDNS.ORG")==0)
		strcpy(service, "dyndns");			
	else if (strcmp(server, "WWW.DYNDNS.ORG(CUSTOM)")==0)
		strcpy(service, "dyndns");			
	else if (strcmp(server, "WWW.DYNDNS.ORG(STATIC)")==0)
		strcpy(service, "dyndns");			
	else if (strcmp(server, "WWW.TZO.COM")==0)
		strcpy(service, "tzo");			
	else if (strcmp(server, "WWW.ZONEEDIT.COM")==0)
		strcpy(service, "zoneedit");
	else if (strcmp(server, "WWW.JUSTLINUX.COM")==0)
		strcpy(service, "justlinux");
	else if (strcmp(server, "WWW.EASYDNS.COM")==0)
		strcpy(service, "easydns");
#ifdef ASUS_DDNS //2007.03.20 Yau add
        else if (strcmp(server, "WWW.ASUS.COM")==0)
                strcpy(service, "dyndns");
#endif
	else strcpy(service, "dyndns");
			
	sprintf(usrstr, "%s:%s", user, passwd);
	
	if (nvram_match("wan_proto", "pppoe") || nvram_match("wan_proto", "pptp") || nvram_match("wan_proto", "l2tp"))
	{
		strcpy(wan_ifname, nvram_safe_get("wan0_pppoe_ifname"));
	}
	else
	{
		strcpy(wan_ifname, nvram_safe_get("wan0_ifname"));
	}	

	dprintf("wan_ifname: %s\n\n\n\n", wan_ifname);

#ifdef ASUS_DDNS //2007.03.20 Yau add
        if (strcmp(server, "WWW.ASUS.COM")==0)
	{
                char *ddns_argv[] = {"ez-ipupdate",
		"-h", host,
		"-s", "ns1.asuscomm.com",
                "-S", service,
		"-D", 
                "-i", wan_ifname,
                "-A", "1",
                NULL};
                pid_t pid;
                                                                                                                                              
                dprintf("ddns update %s %s\n", server, service);

                eval("killall", "ez-ipupdate");
                _eval(ddns_argv, NULL, 0, &pid);
	}
	else
#endif //End of Yau add
	if (strlen(service)>0)
	{
		char *ddns_argv[] = {"ez-ipupdate", 
		"-S", service,
	        "-i", wan_ifname,
 		"-u", usrstr,
		"-h", host,
		"-e", "/sbin/ddns_updated",
		"-b", "/tmp/ddns.cache",
		wild ? "-w" : NULL,
		NULL};	
		pid_t pid;

		dprintf("ddns update %s %s\n", server, service);
		nvram_unset("ddns_cache");
		nvram_unset("ddns_ipaddr");
		nvram_unset("ddns_status");

		eval("killall", "ez-ipupdate");
		_eval(ddns_argv, NULL, 0, &pid);
	}
	return 0;
}

int 
stop_ddns(void)
{
	int ret = eval("killall", "ez-ipupdate");

	dprintf("done\n");
	return ret;
}

int 
start_logger(void)
{		
	pid_t pid;

	if (nvram_match("router_disable", "1"))
		return 0;

	if (nvram_invmatch("log_ipaddr", ""))
	{
		char *syslogd_argv[] = {"syslogd", "-S", "-m", "0", "-O", "/tmp/syslog.log", "-b", "0",
					"-R", nvram_safe_get("log_ipaddr"), "-L", NULL};

#ifdef KERNEL_DBG
		char *klogd_argv[] = {"klogd", "-d", NULL};
#else
		char *klogd_argv[] = {"klogd", "-c", "4", NULL};
#endif

		setenv("TZ", nvram_safe_get("time_zone_x"), 1);

		unlink("/tmp/syslog.log");

		_eval(syslogd_argv, NULL, 0, &pid);
		if (nvram_match("kernel_dbg", "1") || nvram_invmatch("fw_log_x","none"))
			_eval(klogd_argv, NULL, 0, &pid);

	}
	else
	{
//		char *syslogd_argv[] = {"syslogd", "-m", "0", "-s", "200", "-O", "/tmp/syslog.log", "-b", "0", NULL};
		char *syslogd_argv[] = {"syslogd", "-S", "-m", "0", "-O", "/tmp/syslog.log", "-b", "0", NULL};

#ifdef KERNEL_DBG
		char *klogd_argv[] = {"klogd", "-d", NULL};
#else
		char *klogd_argv[] = {"klogd", NULL};
#endif
		setenv("TZ", nvram_safe_get("time_zone_x"), 1);

		unlink("/tmp/syslog.log");

		_eval(syslogd_argv, NULL, 0, &pid);
		if (nvram_match("kernel_dbg", "1") || nvram_invmatch("fw_log_x","none"))
			_eval(klogd_argv, NULL, 0, &pid);
	}
	// remote log not easy to ok
	sleep(1);
	return 0;
}

int
stop_logger(void)
{
	int ret1 = eval("killall", "klogd");
	int ret2 = eval("killall", "syslogd");

	dprintf("done\n");
	return (ret1|ret2);
}
/*
int
chpass(char *user, char *pass)
{
        char cmdbuf[128];
	FILE *fp;

        if(!user)
                user = "admin";
        if(!pass)
                pass = "admin";

#if 0
        memset(cmdbuf, 0, sizeof(cmdbuf));
        sprintf(cmdbuf, "echo '%s::0:0:Adminstrator:/:/bin/sh' > /etc/passwd", user);
        system(cmdbuf);
        memset(cmdbuf, 0, sizeof(cmdbuf));
        sprintf(cmdbuf, "echo '%s:x:0:%s' > /etc/group", user, user);
        system(cmdbuf);
        memset(cmdbuf, 0, sizeof(cmdbuf));
        sprintf(cmdbuf, "chpasswd %s:%s", user, pass);
        system(cmdbuf);
#endif
	if (fp = fopen ("/etc/passwd", "a")) {
		fclose(fp);
		printf("Open /etc/passwd fail\n");
	}

	fprintf("%s::0:0:Adminstrator:/:/bin/sh", user);
	close(fp);

	if (fp = fopen ("/etc/group", "a")) {
		fclose(fp);
		printf("Open /etc/group fail\n");
	}

	fprintf("%s:x:0:%s", user, user);
	close(fp);

	//eval(chpasswd, user, ":", pass);

        return 0;
}

int
start_telnetd()
{
        char *telnetd_argv[] = {"telnetd", NULL};

        system("killall telnetd");

        chpass(nvram_safe_get("http_username"), nvram_safe_get("http_passwd"));
        system("hostname RT-N16");

        return eval(telnetd_argv, NULL, 0, NULL);
        //        return system("telnetd");
}

int
stop_telnetd()
{
        return system("killall telnetd");
}
*/

int 
start_misc(void)
{ 
	char *infosvr_argv[] = {"infosvr", "br0", NULL};
	char *ots_argv[] = {"ots", NULL};	// 2008.01 James. For communicating to utility.
	char *watchdog_argv[] = {"watchdog", NULL};
	pid_t pid;

	_eval(infosvr_argv, NULL, 0, &pid);

	_eval(ots_argv, NULL, 0, &pid);	// 2008.01 James. For communicating to utility.

	_eval(watchdog_argv, NULL, 0, &pid);


	// for all product, fix antdiv
	eval("wl", "antdiv", "0");
	eval("wl", "txant", "0");

	if (nvram_match("wl_radio_x", "0"))
		wl_led_ctrl(0);

	return 0;
}

int
stop_misc(void)
{
	int ret1 = eval("killall", "infosvr");
	
	eval("killall", "ots");
	
	eval("killall", "watchdog");
	//eval("rm", "-f", "/var/run/watchdog.pid");	// 2007.11 James.
	unlink("/var/run/watchdog.pid");	// 2008.08 James.
	
	eval("killall", "ntp");
	stop_telnetd();

	dprintf("done\n");
	return(ret1);
}

int
stop_misc_no_watchdog(void)
{
	int ret1 = eval("killall", "infosvr");
	eval("killall", "ntp");

	dprintf("done\n");
	return(ret1);
}


#ifndef USB_SUPPORT
int start_usb(void)
{
	return 0;
}

int stop_usb(void)
{
	return 0;
}

int hotplug_usb(void)
{
	return 0;
}
#else
int 
start_usb(void)
{
	eval("insmod", "usbcore");
	eval("insmod", "ehci-hcd");
	eval("insmod", "ohci-hcd");
	eval("insmod", "ufsd");
//	eval("insmod", "uhci");
//#ifdef DLM
//#endif

#ifdef CDMA
	eval("insmod", "acm.o");
//	eval("insmod", "usbserial.o", "vendor=0x1165", "product=0x0001");
        eval("insmod", "usbserial.o", "vendor=0x12d1", "product=0x1003"); //Huawei E220
//      eval("insmod", "usbserial.o", "vendor=0x0b05", "product=0x0302"); //Bandluxe C100
//      eval("insmod", "usbserial.o", "vendor=0x0bb4", "product=0x00ce"); //ASUS T500
//      eval("insmod", "usbserial.o", "vendor=0x0421", "product=0x04f0"); //Nokia 95
//      eval("insmod", "usbserial.o", "vendor=0x0fce", "product=0xd039"); //SE K800

#endif

#ifdef LINUX26
	eval("mount", "-t", "usbfs", "none", "/proc/bus/usb");
#else
	eval("mount", "-t", "usbdevfs", "none", "/proc/bus/usb/");
#endif

#ifdef U2EC
	char *u2ec_argv[] = {"u2ec", NULL};
	pid_t pid;
	_eval(u2ec_argv, NULL, 0, &pid);
#else
	semaphore_create();
	nvram_set("MFP_busy", "0");
#endif

#ifdef PRINTER_SUPPORT
#ifdef PARPORT_SUPPORT	
	symlink("/dev/printers/0", "/dev/lp0");
//	symlink("/dev/lp0", "/tmp/lp0");
//	eval("insmod", "parport.o");
//	eval("insmod", "parport_splink.o");
	//eval("insmod", "lp.o");
#endif		
	//eval("insmod", "printer.o");
	umask(0000);
	mkdir("/dev/usb", 0777);
	eval("mknod", "/dev/usb/lp0", "c", "180", "0");
	eval("insmod", "usblp.o");
	mkdir("/var/state", 0777);
	mkdir("/var/state/parport", 0777);
	mkdir("/var/state/parport/svr_statue", 0777);

	{
	char *lpd_argv[]={"lpd", NULL};
		pid_t pid;

	sleep(2);
	_eval(lpd_argv, ">/dev/null", 0, &pid);	
	}

#endif	

#ifdef MASSSTORAGE_SUPPORT
	if (nvram_invmatch("usb_ftpenable_x", "0"))
	{
		eval("insmod", "scsi_mod.o");
		eval("insmod", "sd_mod.o");
//		sleep(1);	// 2008.06 James. For some flashes, must delay for detecting.
		eval("insmod", "usb-storage.o");
		umask(0000);			// added by Jiahao for WL500gP
		mkdir("/tmp/harddisk", 0777);
	}	
#endif

#ifdef AUDIO_SUPPORT
//	eval("insmod", "soundcore.o");
//	eval("insmod", "audio.o");
//	start_audio();
#endif
#ifdef WEBCAM_SUPPORT	
//	if (nvram_invmatch("usb_webenable_x", "0"))
//	{	
//		eval("insmod", "videodev.o");

		// link video 
//		symlink("/dev/v4l/video0", "/dev/video");
//		start_rcamd();
//	}
#endif


	return 0;	// 2007.11 James.
}

int
stop_usb(void)
{
#ifdef MASSSTORAGE_SUPPORT
	if (nvram_invmatch("usb_ftpenable_x", "0"))
	{
#ifdef DLM
		eval("killall", "snarf");
		eval("killall", "ctorrent");
		eval("killall", "rtorrent");
		eval("killall", "giftd");
		eval("killall", "dmathined");
//		eval("killall", "dms");
		eval("killall", "ushare");
		eval("killall", "-SIGKILL", "nmbd");
		eval("killall", "-SIGKILL", "smbd");
		eval("killall", "-SIGKILL", "vsftpd");
#endif
		eval("rmmod", "usb-storage");
		eval("rmmod", "sd_mod");
		eval("rmmod", "scsi_mod");
	}
#endif
#ifdef WEBCAM_SUPPORT	
	if (nvram_invmatch("usb_webenable_x", "0"))
	{
		stop_rcamd();	
		eval("killall", "rcamd");
		eval("killall", "sendmail");
		eval("rmmod", "pwc");
		eval("rmmod", "ov511_decomp");
		eval("rmmod", "ov518_decomp");
		eval("rmmod", "ov51x");
		eval("rmmod", "i2c-core");
		eval("rmmod", "videodev");
	}
#endif
#ifdef AUDIO_SUPPORT
	eval("rmmod", "audio");
	eval("rmmod", "soundcore");
	stop_audio();
#endif
#ifdef PRINTER_SUPPORT	
	eval("killall", "lpd");
	//eval("rmmod", "printer");
	eval("rmmod", "usblp");
#ifdef PARPORT_SUPPORT
	//eval("rmmod", "lp.o");
	eval("rmmod", "parport_splink.o");
	eval("rmmod", "parport.o");
#endif
#endif	
#ifdef DLM
//	eval("rmmod", "ehci-hcd");
#endif
	eval("rmmod", "usb-ohci");
	eval("rmmod", "usbcore");
	return 0;	// 2007.11 James.
}

void start_script(int *new_disc)	// added by Jiahao for WL500gP
{
	char nvram_str[32];
	int i=0;
#ifdef DLM
	char run_path[64];
	char runbuf[256];
	int run_count=0;
#endif
#ifdef WCN
	char run_path_wcn[128];
	char run_dir_wcn[128];

	int disc_index=0;
	int j=0;
	int wcn_count=0;
#endif

	for(i=0;i<2;i++)
	{
		if ( *(new_disc+i)==1 )
		{
#ifdef DLM
			memset(nvram_str, 0, sizeof(nvram_str));
			sprintf(nvram_str, "usb_disc%d_path0", i);
			memset(run_path, 0, sizeof(run_path));
			strcpy(run_path, nvram_safe_get(nvram_str));
#endif

#ifdef WCN
			memset(run_path_wcn, 0, sizeof(run_path_wcn));
			memset(run_dir_wcn, 0, sizeof(run_dir_wcn));
			sprintf(nvram_str, "usb_disc%d_index", i);
			disc_index=atoi(nvram_safe_get(nvram_str));
			for (j=0; j<disc_index; j++)
			{
				sprintf(nvram_str, "usb_disc%d_path%d", i, j);
				sprintf(run_path_wcn, "%s/SMRTNTKY/WSETTING.WFC", nvram_safe_get(nvram_str));
				sprintf(run_dir_wcn, "%s/SMRTNTKY/", nvram_safe_get(nvram_str));

				if (check_if_dir_exist(run_dir_wcn)==1) {

					if (check_if_file_exist(run_path_wcn)==1)
					{
						memset(run_path_wcn, 0, sizeof(run_path_wcn));
						strcpy(run_path_wcn, nvram_safe_get(nvram_str));
						wcn_count++;
						break;
					}
					else {
						memset(run_path_wcn, 0, sizeof(run_path_wcn));
					}
				} else {
					memset(run_dir_wcn, 0, sizeof(run_dir_wcn));
				}
			}
#endif
		}
		else {
			continue;
	}
#ifdef WCN
		if (wcn_count==1 && nvram_invmatch("wps_config_state", "1") &&
		    !nvram_match("x_Setting", "1"))/* Cherry Cho modified in 2007/4/13. */ 
		{
			printf("start wfc_parser\n");
			wfc_parser(run_path_wcn);
		}
#endif
#ifdef DLM
		if (strcmp(run_path, "")!=0)
		{
			if (strlen(nvram_safe_get("run_prog"))==0)
				continue;

			memset(runbuf, 0, sizeof(runbuf));
			sprintf(runbuf, "%s/%s", run_path, nvram_safe_get("run_prog"));
			if (check_if_file_exist(runbuf)==1 && run_count==0)
			{
				run_count++;
				logmessage("USB storage", "execute the user specified script");
				eval(runbuf);
			}
		}
#endif
	}
}

#ifdef DLM
void write_ftpd_conf()
{
	FILE *fp;
	char user[64], user1[64], password[64], path[64];
	char tmpstr[64];
	char rright[384], wright[384], maxuser[16];
	int snum, unum, i, j;
	char *test_path=(char *)malloc(64);
	char *tmp1=NULL;
	char root_path[64];

	/* write /etc/vsftpd.conf */
	fp=fopen("/tmp/vsftpd.conf", "w");
	if (fp==NULL) return;
// 2007.11 James {
	// new vsftpd
	if(nvram_match("st_ftp_mode", "2"))
		fprintf(fp, "anonymous_enable=NO\n");
	else{
		fprintf(fp, "anonymous_enable=YES\n");
		fprintf(fp, "anon_upload_enable=YES\n");
		fprintf(fp, "anon_mkdir_write_enable=YES\n");
		fprintf(fp, "anon_other_write_enable=YES\n");
	}//*/
	
	// old vsftpd
	/*fprintf(fp, "anonymous_enable=YES\n");
	fprintf(fp, "anon_upload_enable=YES\n");
	fprintf(fp, "anon_mkdir_write_enable=YES\n");
	fprintf(fp, "anon_other_write_enable=YES\n");//*/
// 2007.11 James }
	fprintf(fp, "nopriv_user=root\n");
	fprintf(fp, "write_enable=YES\n");
	fprintf(fp, "local_enable=YES\n");
	fprintf(fp, "chroot_local_user=YES\n");
	fprintf(fp, "local_umask=000\n");
	fprintf(fp, "dirmessage_enable=NO\n");
	fprintf(fp, "xferlog_enable=NO\n");
	fprintf(fp, "syslog_enable=NO\n");
	fprintf(fp, "connect_from_port_20=YES\n");
	fprintf(fp, "listen=YES\n");
	fprintf(fp, "pasv_enable=YES\n");
	fprintf(fp, "ssl_enable=NO\n");
	fprintf(fp, "tcp_wrappers=NO\n");
	strcpy(maxuser, nvram_safe_get("st_max_user"));
	fprintf(fp, "max_clients=%s\n", maxuser);
	fprintf(fp, "ftp_username=anonymous\n");
	fprintf(fp, "ftpd_banner=Welcome to the %s FTP service.\n", nvram_safe_get("computer_name"));//2006_07_03_Roly modify WL500gp to WL500W
//#ifdef LANGUAGE_TW
	fprintf(fp, "enable_iconv=YES\n");
	if(nvram_match("ftp_lang", "TW")){
		fprintf(fp, "remote_charset=cp950\n");
	}
	else if(nvram_match("ftp_lang", "CN")){
		fprintf(fp, "remote_charset=cp936\n");
	}
	else if(nvram_match("ftp_lang", "RU")){
		fprintf(fp, "remote_charset=cp1251\n");
	}
	else if(nvram_match("ftp_lang", "CZ")){
		fprintf(fp, "remote_charset=cp1250\n");
	}


	/*else if(nvram_match("ftp_lang", "KR")){
		fprintf(fp, "remote_charset=cp949\n");
	}//*/
//#endif
// 2008.04 James. {
/*#ifdef LANGUAGE_KR
	fprintf(fp, "enable_iconv=YES\n");
	fprintf(fp, "remote_charset=cp949\n");
#endif
#ifdef LANGUAGE_JP
	fprintf(fp, "enable_iconv=YES\n");
	fprintf(fp, "remote_charset=cp932\n");
#endif//*/
	/*else if(nvram_match("ftp_lang", "JP")){
		fprintf(fp, "remote_charset=cp932\n");
	}//*/
// 2008.04 James. }
	fclose(fp);

	get_first_partition(test_path);
	nvram_set("first_partition", test_path);
	free(test_path);
}

int
start_ftpd()	// added by Jiahao for WL500gP
{
	int ret=0;
	
	if (nvram_match("ftp_enable", "0")) return 1;

	//eval("killall", "-SIGKILL", "vsftpd");
	stop_ftp();	// 2007.10 James.
	write_ftpd_conf();

	//if (nvram_match("st_ftp_modex", "1"))
	if(nvram_match("st_ftp_mode", "1"))	// 2007.10 James
		//printf("ftp mode: login to first partition\n");
		printf("ftp mode: anonymous login...\n");	// 2007.11 James.
	//else if (nvram_match("st_ftp_modex", "2"))
	else if(nvram_match("st_ftp_mode", "2"))	// 2007.10 James
		//printf("ftp mode: login to first matched shared node\n");
		printf("ftp mode: account login...\n");	// 2007.11 James.
	
	// Can't use eval() here
	ret=system("/sbin/vsftpd&");	// 2007.10 James. chang the absolute path.
	if(!ret)
	{
		logmessage("FTP Server", "daemon is started");
		return 0;
	}
	else
		return 1;
}

int
test_user(char *target, char *pattern)	// added by Jiahao for WL500gP
{
	char s[384];
	char p[32];
	char *start;
	char *pp;
	strcpy(s, target);
	strcpy(p, pattern);
	start = s;
	while (pp=strchr(start, ';'))
	{
		*pp='\0';
		if(! strcmp(start, p))
			return 1;
		start=pp+1;
	}
	return 0;
}
#endif

int
hotplug_usb_webcam(char *product, int webflag)
{
	char *rcamd_argv[]={"rcamd", 
				"-p", nvram_safe_get("usb_webactivex_x"),
				"-s", "0",
				"-z", nvram_safe_get("time_zone_x"),
				"-a", nvram_safe_get("usb_websecurity_x"),
				NULL, NULL,	// Model -t
				NULL, NULL,	// Record Time -r
				NULL, NULL, 	// Image Size -f
				NULL, NULL, 	// Sense Vlaue -m 
				NULL, NULL, 	// Sense Limit -c
				NULL, NULL,
				NULL};
	char **arg;
	pid_t pid;

	if (nvram_match("usb_webenable_x", "0") || strlen(product)==0 || webflag==0) return;
	for (arg = rcamd_argv; *arg; arg++);
	
	if (webflag == WEB_PWCWEB)
	{
		eval("insmod", "pwc.o", "power_save=0");
		nvram_set("usb_webdriver_x", "0");

		*arg++ = "-t";
		*arg++ = "0";
	}
	else
	{
		eval("insmod", "i2c-core.o");
		eval("insmod", "ov51x.o");
		eval("insmod", "ov511_decomp.o");
		eval("insmod", "ov518_decomp.o");
		nvram_set("usb_webdriver_x", "1");

		if (strstr(product, "8519"))
		{
			*arg++ = "-t";
			*arg++ = "2";
		}
		else
		{			
			*arg++ = "-t";
			*arg++ = "1";
		}
	}

	// start web cam
	if (nvram_match("usb_webmode_x", "0")) // ActiveX only
	{
		*arg++ = "-r";
		*arg++ = "60";
	}
	else 
	{
		*arg++ = "-r";
		*arg++ = "0";
	}
		
	// image size
	if (nvram_match("usb_webimage_x", "0"))
	{
		*arg++ = "-f";
		if (webflag==WEB_PWCWEB) *arg++="320x240";
		else *arg++="640x480";
	}
	else if (nvram_match("usb_webimage_x", "1"))
	{
		*arg++ = "-f";
		*arg++="320x240";
	}
	else if (nvram_match("usb_webimage_x", "2"))
	{
		*arg++ = "-f";
		if (webflag==WEB_PWCWEB) *arg++="160x120";
		else *arg++="176x144";
	}
	else
	{
		*arg++ = "-f";
		*arg++ = "80x60";
	}


	if (nvram_match("usb_websense_x", "0"))
	{	
		*arg++ = "-m";
		*arg++ = "150";
		*arg++ = "-c";
		*arg++ = "100";
	}
	else if (nvram_match("usb_websense_x", "1"))
	{
		*arg++ = "-m";
		*arg++ = "100";
		*arg++ = "-c";
		*arg++ = "100";
	}
	else
	{	
		*arg++ = "-m";
		*arg++ = "50";
		*arg++ = "-c";
		*arg++ = "100";
	}
	

	//*arg++="-d";
	//*arg++="7";
	mkdir("/tmp/webcam", 0777);	
	chdir("/tmp/webcam");

	symlink("/www/Advanced_ShowTime_Widzard.asp", "/tmp/webcam/index.asp");
	symlink("/www/Advanced_ShowTime_Widzard.asp", "/tmp/webcam/ShowWebCam.asp");
	symlink("/www/ShowWebCamPic.asp", "/tmp/webcam/ShowWebCamPic.asp");
	symlink("/www/graph", "/tmp/webcam/graph");
	symlink("/www/general.js", "/tmp/webcam/general.js");
	symlink("/www/overlib.js", "/tmp/webcam/overlib.js");
	symlink("/www/style.css", "/tmp/webcam/style.css");
	symlink("/www/netcam_mfc_activeX.cab", "/tmp/webcam/netcam_mfc_activeX.cab");
	symlink("/var/tmp/display.jpg", "/tmp/webcam/display.jpg");

	//char *httpd_argv[]={"httpd", 
	//			nvram_safe_get("wan0_ifname"), 
	//			nvram_safe_get("usb_webhttpport_x"),
	//			NULL};
	//_eval(httpd_argv, NULL, 0, &pid);
	eval("httpd", nvram_safe_get("wan0_ifname"), nvram_safe_get("usb_webhttpport_x"));
	chdir("/");
	_eval(rcamd_argv, ">/dev/null", 0, NULL);

	return 0;
}

int
remove_webcam_main(int webflag)
{
	if (webflag == WEB_PWCWEB)
	{
		eval("rmmod", "pwc");
	}
	else
	{
		eval("rmmod", "i2c-core");
		eval("rmmod", "ov511_decomp");
		eval("rmmod", "ov518_decomp");
		eval("rmmod", "ov51x");
	}
	nvram_set("usb_webdriver_x", "");
	return 0;
}


int
remove_usb_webcam(char *product, int webflag)
{
	char pidfile[32];
	sprintf(pidfile, "/var/run/httpd-%s.pid", nvram_safe_get("usb_webhttpport_x"));

	kill_pidfile(pidfile);
	kill_pidfile("/var/run/rcamd.pid");

	if (webflag == WEB_PWCWEB)
	{
		eval("rmmod", "pwc");
	}
	else
	{
		eval("rmmod", "i2c-core");
		eval("rmmod", "ov511_decomp");
		eval("rmmod", "ov518_decomp");
		eval("rmmod", "ov51x");
	}
	nvram_set("usb_webdriver_x", "");

	return 0;
}


int
start_rcamd(void)
{
	char *rcamd_argv[] = {"rcamdmain", NULL};
	pid_t pid;

	_eval(rcamd_argv, NULL, 0, &pid);
	return 0;
}

int
stop_rcamd(void)
{
	int ret = eval("killall", "rcamdmain");
	return ret;	// 2007.11 James.
}

/* remove usb mass storage */
int
remove_usb_mass(char *product)
{
	nvram_set("usb_storage_device_remove", "");	// 2007.12 James.
#ifdef REMOVE
	if(product!=NULL) 
		logmessage("USB storage", product);
	else
		logmessage("USB storage", "NULL");
#endif

//	hotplug_usb_mass("");
// 2007.12 James {
#ifdef DLM
	char *message;
	unsigned int len = 0;
	int result = 0;
	
printf("\n*** start to umount disk! ***\n");
// 2008.06 James. {

	if (nvram_match("usb_disc0_port", "0")) {
		nvram_set("usb_disc0_pid", "");
		nvram_set("usb_disc0_safely_removed", "0");
		umount_disc_parts_rc(1);
	}
	if (nvram_match("usb_disc1_port", "0")) {
		nvram_set("usb_disc1_pid", "");
		nvram_set("usb_disc1_safely_removed", "0");
		umount_disc_parts_rc(2);
	}

//	umount_disc_parts_rc(1);
//	umount_disc_parts_rc(2);
	
// 2008.06 James. }
	
	sync();	// 2008.04 James.
	
printf("\n*** start to rmmod usb-storage! ***\n");
	//result = system("rmmod usb-storage");
	result = eval("/sbin/rmmod", "usb-storage");	// 2008.04 James.
	
printf("\n*** start to insmod usb-storage! ***\n");
	//result = system("insmod usb-storage");
	result = eval("/sbin/insmod", "usb-storage");	// 2008.04 James.
	
printf("\n*** start to mount the plugged disk! ***\n");
	if(strlen(nvram_safe_get("usb_disc0_port")) > 0 || strlen(nvram_safe_get("usb_disc1_port")) > 0)
		hotplug_usb_mass("");
#endif
// 2007.12 James }
	return 0;
}

int
remove_storage_main(void)
{
	remove_usb_mass(NULL);
	return 0;
}

#define MOUNT_VAL_FAIL 	0
#define MOUNT_VAL_RONLY	1
#define MOUNT_VAL_RW 	2

#ifdef LINUX26
int
mount_r(char *usb_part, char *usb_file_part)
{
        struct mnt_param {
                char *type;
                unsigned long flags;
                char *opt;
                int retval;
        } *pm, m[] = {
#ifdef LINUX26
                {"ext3",    MS_SYNCHRONOUS, NULL, MOUNT_VAL_RW},
                {"ext2",    MS_SYNCHRONOUS, NULL, MOUNT_VAL_RW},
                {"vfat",    !MS_SYNCHRONOUS, "utf8=1,umask=0000,usefree,shortname=mixed", MOUNT_VAL_RW},
                {"msdos",   MS_SYNCHRONOUS, "utf8=1,umask=0000", MOUNT_VAL_RW},
                {"ufsd",    MS_SYNCHRONOUS, NULL, MOUNT_VAL_RW},
//                {"ntfs-3g", 0, "umask=0000,locale=en_US.UTF-8", MOUNT_VAL_RW},
//                {"ntfs",    MS_SYNCHRONOUS, "utf8=1,umask=0000", MOUNT_VAL_RONLY},
#else   /* !KERNEL_2_6 */
                {"ext2",    MS_SYNCHRONOUS, NULL, MOUNT_VAL_RW},
                {"vfat",    MS_SYNCHRONOUS, "utf8=1,umask=0000,mode=0777", MOUNT_VAL_RW},
                {"msdos",   MS_SYNCHRONOUS, "utf8=1,umask=0000,mode=0777", MOUNT_VAL_RW},
                {"ntfs",    MS_SYNCHRONOUS, "utf8=1,umask=0000", MOUNT_VAL_RONLY},
#endif  /* KERNEL_2_6 */
                {NULL, 0, NULL, 0},
        };
        int rc, wb = 0;                 /* if wb=1, disable sync flag */
        char msg[64], *p;
        unsigned long flags;

        /* Disable sync flag if and only if usb_writeback is set and is non-zero value. */
        if (nvram_invmatch("usb_writeback", "0"))
                wb = 1;

        pm = m;
        while (pm->type != NULL)        {
//		printf("dbg: pm->type = %s\n", pm->type);
                flags = pm->flags;
                if (wb) flags &= ~(MS_SYNCHRONOUS);

                if (!(strcmp(pm->type, "ufsd") == 0))        {
                        rc = mount(usb_part, usb_file_part, pm->type, flags, pm->opt);
                } else {
                        /* Both the mount command of busybox and mount() system call can't mount 
 *                          * filesystem that base on FUSE. e.g. NTFS-3g
 *                                                   */
/*
                        char scmd[] =   "mount.ntfs-3g /dev/sdaXXX /tmp/harddisk/partXXXXXXXXXX" \
                                        "-o XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX";

                        sprintf(scmd, "mount.ntfs-3g %s %s -o %s", usb_part, usb_file_part, pm->opt);
                        rc = system(scmd);
*/
			char scmd[64];
			int ret_chk=0;
//                        sprintf(scmd, "mount -t ufsd -o iocharset=utf8 -o force -o trace=all -o force %s %s", usb_part, usb_file_part);
			ret_chk = eval("chkntfs", usb_part, "-f", "-a");
//			printf("dbg: chkntfs = %d\n");
//                        sprintf(scmd, "mount -t ufsd -o nls=utf8 -o trace=all -o force %s %s", usb_part, usb_file_part);
//                        sprintf(scmd, "mount -t ufsd -o iocharset=utf8 -o trace=all -o sparse %s %s", usb_part, usb_file_part);
//                        sprintf(scmd, "mount -t ufsd -o iocharset=utf8 -o sparse %s %s", usb_part, usb_file_part);
                        sprintf(scmd, "mount -t ufsd -o iocharset=utf8 -o nobuf %s %s", usb_part, usb_file_part);
//                        sprintf(scmd, "mount -t ufsd -o iocharset=utf8 -o force %s %s", usb_part, usb_file_part);
                        rc = system(scmd);
//			eval("mount", "-t", "ufsd", "-o", "iocharset=utf8", "-o", "trace=all", "-o", "sparse","-o", "force", usb_part, usb_file_part);
//			rc = 0;
                }
                
                if (rc == 0)    {
                        chmod(usb_file_part, 0777);
                        sprintf(msg, "%s fs mounted to %s\n", pm->type, usb_file_part);
                        logmessage("USB storage", msg);
                        eval("/sbin/test_of_var_files_in_mount_path", usb_file_part); // 2009.05 James.
                        printf("%s fs mounted to %s\n", pm->type, usb_file_part);
			if(strcmp(usb_file_part, "/tmp/harddisk/part0")==0) {
				if (strncmp(pm->type, "ufsd", 4) == 0)
					nvram_set("mnt_type", "ntfs");
				else
					nvram_set("mnt_type", pm->type);
			}
//			printf("dbg: retval = %d\n", pm->retval);
                        return pm->retval;
                }

                ++pm;
        }

        return MOUNT_VAL_FAIL;
}

#else
int
mount_r(char *usb_part, char *usb_file_part)
{
	char msg[64];
// 2007.12 James {
	if(!mount(usb_part, usb_file_part, "ext3", MS_SYNCHRONOUS, NULL)) 
	{
		chmod(usb_file_part, 0777);
		sprintf(msg, "ext3 fs mounted to %s\n", usb_file_part);		
		logmessage("USB storage", msg);
		printf("ext3 fs mounted to %s\n", usb_file_part);
		eval("/sbin/test_of_var_files_in_mount_path", usb_file_part);
		return MOUNT_VAL_RW;
	}
// 2007.12 James }
	if(!mount(usb_part, usb_file_part, "ext2", MS_SYNCHRONOUS, NULL)) 
	{
		chmod(usb_file_part, 0777);
		sprintf(msg, "ext2 fs mounted to %s\n", usb_file_part);		
#ifdef DLM
		logmessage("USB storage", msg);
#endif
		printf("ext2 fs mounted to %s\n", usb_file_part);
		eval("/sbin/test_of_var_files_in_mount_path", usb_file_part);	// 2007.12. James.
		return MOUNT_VAL_RW;
	}
/*	marked by Jiahao for WL500gP
#ifdef LANGUAGE_EN
	if(!mount(usb_part, usb_file_part, "vfat", MS_SYNCHRONOUS, NULL)) 
	{
		sprintf(msg, "vfat fs mounted to %s\n", usb_file_part);		
		logmessage("USB storage", msg);
		printf("vfat fs mounted to %s\n", usb_file_part);
		return MOUNT_VAL_RW;
	}
#endif
#ifdef LANGUAGE_TW
	if(!mount(usb_part, usb_file_part, "vfat", MS_SYNCHRONOUS, "codepage=950,iocharset=cp950")) 
	{
		umask(0022);
		sprintf(msg, "vfat fs mounted to %s\n", usb_file_part);		
		logmessage("USB storage", msg);
		printf("vfat fs mounted to %s\n", usb_file_part);
		return MOUNT_VAL_RW;
	}
#endif
#ifdef LANGUAGE_CN
	if(!mount(usb_part, usb_file_part, "vfat", MS_SYNCHRONOUS, "codepage=936,iocharset=cp936")) 
	{
		umask(0022);
		sprintf(msg, "vfat fs mounted to %s\n", usb_file_part);		
		logmessage("USB storage", msg);
		printf("vfat fs mounted to %s\n", usb_file_part);
		return MOUNT_VAL_RW;
	}
#endif
#ifdef LANGUAGE_KR
	if(!mount(usb_part, usb_file_part, "vfat", MS_SYNCHRONOUS, "codepage=949,iocharset=cp949")) 
	{
		umask(0022);
		sprintf(msg, "vfat fs mounted to %s\n", usb_file_part);		
		logmessage("USB storage", msg);
		printf("vfat fs mounted to %s\n", usb_file_part);
		return MOUNT_VAL_RW;
	}
#endif
#ifdef LANGUAGE_JP
	if(!mount(usb_part, usb_file_part, "vfat", MS_SYNCHRONOUS, "codepage=932,iocharset=cp932")) 
	{
		umask(0022);
		sprintf(msg, "vfat fs mounted to %s\n", usb_file_part);		
		logmessage("USB storage", msg);
		printf("vfat fs mounted to %s\n", usb_file_part);
		return MOUNT_VAL_RW;
	}
#endif
*/
	// added by Jiahao for WL500gP
	
	if(!mount(usb_part, usb_file_part, "vfat", MS_SYNCHRONOUS, "utf8=1,umask=0000,mode=0777")) 
	{
		sprintf(msg, "vfat fs mounted to %s\n", usb_file_part);		
#ifdef DLM
		logmessage("USB storage", msg);
#endif
		printf("vfat fs mounted to %s\n", usb_file_part);
		eval("/sbin/test_of_var_files_in_mount_path", usb_file_part);	// 2007.12. James.
		return MOUNT_VAL_RW;
	}
	if(!mount(usb_part, usb_file_part, "msdos", MS_SYNCHRONOUS, "utf8=1,umask=0000,mode=0777"))
	{
		sprintf(msg, "msdos fs mounted to %s\n", usb_file_part);	
#ifdef DLM
		logmessage("USB storage", msg);
#endif
		printf("msdoc fs mounted to %s\n", usb_file_part);
		eval("/sbin/test_of_var_files_in_mount_path", usb_file_part);	// 2007.12. James.
		return MOUNT_VAL_RW;
	}
	if(!mount(usb_part, usb_file_part, "ntfs", MS_SYNCHRONOUS, "utf8=1,umask=0000"))
	{
		sprintf(msg, "ntfs(ro) fs mounted to %s\n", usb_file_part);	
#ifdef DLM
		logmessage("USB storage", msg);
#endif
		printf("ntfs fs mounted to %s\n", usb_file_part);
		return MOUNT_VAL_RONLY;
	}
/*	marked by Jiahao for WL500gP
#ifdef LANGUAGE_EN
	if(!mount(usb_part, usb_file_part, "ntfs", MS_SYNCHRONOUS, NULL))
	{
		sprintf(msg, "ntfs(ro) fs mounted to %s\n", usb_file_part);	
		logmessage("USB storage", msg);
		printf("ntfs fs mounted to %s\n", usb_file_part);
		return MOUNT_VAL_RONLY;
	}
#endif
#ifdef LANGUAGE_TW
	if(!mount(usb_part, usb_file_part, "ntfs", MS_SYNCHRONOUS, "iocharset=cp932"))
	{
		sprintf(msg, "ntfs(ro) fs mounted to %s\n", usb_file_part);	
		logmessage("USB storage", msg);
		printf("ntfs fs mounted to %s\n", usb_file_part);
		return MOUNT_VAL_RONLY;
	}
#endif
#ifdef LANGUAGE_CN
	if(!mount(usb_part, usb_file_part, "ntfs", MS_SYNCHRONOUS, "iocharset=cp936"))
	{
		sprintf(msg, "ntfs(ro) fs mounted to %s\n", usb_file_part);	
		logmessage("USB storage", msg);
		printf("ntfs fs mounted to %s\n", usb_file_part);
		return MOUNT_VAL_RONLY;
	}
#endif
#ifdef LANGUAGE_KR
	if(!mount(usb_part, usb_file_part, "ntfs", MS_SYNCHRONOUS, "iocharset=cp949"))
	{
		sprintf(msg, "ntfs(ro) fs mounted to %s\n", usb_file_part);	
		logmessage("USB storage", msg);
		printf("ntfs fs mounted to %s\n", usb_file_part);
		return MOUNT_VAL_RONLY;
	}
#endif
#ifdef LANGUAGE_JP
	if(!mount(usb_part, usb_file_part, "ntfs", MS_SYNCHRONOUS, "iocharset=cp950"))
	{
		sprintf(msg, "ntfs(ro) fs mounted to %s\n", usb_file_part);	
		logmessage("USB storage", msg);
		printf("ntfs fs mounted to %s\n", usb_file_part);
		return MOUNT_VAL_RONLY;
	}
#endif
*/
	return MOUNT_VAL_FAIL;
}
#endif

#ifdef DLM
int calc_crc32( const char *fname, unsigned long *crc ) {
    FILE *in;           /* input file */
    unsigned char buf[BUFSIZ]; /* pointer to the input buffer */
    size_t i, j;        /* buffer positions*/
    int k;              /* generic integer */
    unsigned long tmpcrc=0xFFFFFFFF;

    /* open file */
    if((in = fopen(fname, "rb")) == NULL) return -1;

    /* loop through the file and calculate CRC */
    while( (i=fread(buf, 1, BUFSIZ, in)) != 0 ){
        for(j=0; j<i; j++){
            k=(tmpcrc ^ buf[j]) & 0x000000FFL;
            tmpcrc=((tmpcrc >> 8) & 0x00FFFFFFL) ^ crcs[k];
        }
    }
    fclose(in);
    *crc=~tmpcrc; /* postconditioning */
    return 0;
}

// judge if the existed application files are correct.
int
ckeck_apps_completeness(const char *dirname)
{
	FILE *fp=NULL;
	FILE *fp2=NULL;
	char line[256];
	char line2[256];
//	char listfilename[64]="/apps/list";
//	char crcfilename[64]="/apps/crc";
	char listfilename[64]="/tmp/harddisk/part0/.apps/list";
	char crcfilename[64]="/tmp/harddisk/part0/.apps/crc";

	char appsdir[64];
	char tmp[256];
	char crc_str[9];
	unsigned long crc;

	if (nvram_match("dm_dbg", "1")) 
		return 1;
	
	memset(appsdir, 0, sizeof(appsdir));
	sprintf(appsdir, "%s%s", dirname, "/.apps");

	if ((fp=fopen(listfilename, "r"))==NULL)
	{
		printf("Cannot find %s.\n", listfilename);
		printf("Make sure it's available.\n");
		return 0;
	}
	
	if ((fp2=fopen(crcfilename, "r"))==NULL)
	{
		printf("Cannot find %s.\n", crcfilename);
		printf("Make sure it's available.\n");
		fclose(fp);
		return 0;
	}
	
	printf("starting to check apps files.\n");
	while (!feof(fp))
	{
		if (fgets(line,sizeof(line),fp)==NULL) break;	/* end-of-file */
		if (strlen(line)<3) continue;			/* line empty */

		memset(tmp, 0, sizeof(tmp));
		sprintf(tmp, "%s%s", appsdir, line+1);		/* file path */
		tmp[strlen(tmp)-1]='\0';
		printf("FILE: %s\n", tmp);
		if(calc_crc32(tmp, &crc) != 0)
		{
			printf("Error reading file %s.\n", tmp);
			sprintf(tmp, "rm -rf %s", appsdir);
			system(tmp);
			fclose(fp);
			fclose(fp2);
			return 0;
		}
		
		if (fgets(line2,sizeof(line2),fp2)==NULL)
		{
			sprintf(tmp, "rm -rf %s", appsdir);
			system(tmp);
			fclose(fp);
			fclose(fp2);
			return 0;
		}
		memset(crc_str, 0, sizeof(crc_str));
		sprintf(crc_str, "%08lX", crc);
		printf("CRC32 now: %s\n", crc_str);

		line2[strlen(line2)-1]='\0';
		printf("CRC32 org: %s\n", line2);
		
		if(strcmp(crc_str, line2)!=0)
		{
			sprintf(tmp, "rm -rf %s", appsdir);
			system(tmp);
			fclose(fp);
			fclose(fp2);
			return 0;
		}
	}

	fclose(fp);
	fclose(fp2);
	return 1;
}

// judge if there are application files.
int
ckeck_apps_completeness_pre(const char *dirname)
{
//return 1;
	FILE *fp=NULL;
	FILE *fp2=NULL;
	char line[256];
	char line2[256];
//	char listfilename[64]="/apps/list";
//	char crcfilename[64]="/apps/crc";
	char listfilename[64]="/tmp/harddisk/part0/.apps/list";
	char crcfilename[64]="/tmp/harddisk/part0/.apps/crc";
	char appsdir[64];
	char tmp[256];
	unsigned long crc;

	if (nvram_match("dm_dbg", "1")) 
		return 1;

	memset(appsdir, 0, sizeof(appsdir));
	sprintf(appsdir, "%s%s", dirname, "/.apps");

//	printf("dbg: check files\n");
	if (!(fp=fopen(listfilename, "r")))
	{
		printf("Cannot find %s.\n", listfilename);
		printf("Make sure it's available.\n");
		
		return 0;
	}

	while (!feof(fp))
	{
		if (fgets(line,sizeof(line),fp)==NULL) break;	/* end-of-file */
		if (strlen(line)<3) continue;			/* line empty */

		memset(tmp, 0, sizeof(tmp));
		sprintf(tmp, "%s%s", appsdir, line+1);		/* file path */
		tmp[strlen(tmp)-1]='\0';
		printf("FILE: %s\n", tmp);

		if(!(fp2 = fopen(tmp, "rb")))
		{
			printf("Error reading file %s.\n", tmp);
			memset(tmp, 0, sizeof(tmp));
			sprintf(tmp, "rm -rf %s", appsdir);
			system(tmp);
			fclose(fp);
			
			return 0;
		}
		fclose(fp2);
	}

	fclose(fp);
	
	return 1;
}
#endif

#ifdef WCN
char ibuf2[8192];

int
channels_in_country(char *abbrev, int channels[])
{
	int i, j, unit;
	wl_channels_in_country_t *cic = (wl_channels_in_country_t *)ibuf2;
	char tmp[100], prefix[] = "wlXXXXXXXXXX_";
	char *name;
	channel_info_t ci;

	if ((unit = atoi(nvram_safe_get("wl_unit"))) < 0)
		return -1;

	snprintf(prefix, sizeof(prefix), "wl%d_", unit);
	name = nvram_safe_get(strcat_r(prefix, "ifname", tmp));

	i = 0;
	channels[i++] = 0;
	channels[i] = -1;

	if (strlen(abbrev)==0) return 0;

	cic->buflen = sizeof(ibuf2);
	strcpy(cic->country_abbrev, abbrev);
	cic->band = WLC_BAND_2G;
	cic->count = 0;

	if (wl_ioctl(name, WLC_GET_CHANNELS_IN_COUNTRY, cic, cic->buflen) == 0)
	{
		if (cic->count == 0)
			return 0;

		for(j = 0; j < cic->count; j++)
		{
			channels[i++] = cic->channel[j];
		}

		channels[i] = -1;
	}
	return cic->count;
}

int
channels_in_country_asus(char *abbrev, int channels[])
{
	int i, j;
	wl_channels_in_country_t *cic = (wl_channels_in_country_t *)ibuf2;
	char var[256], *next;
	int log_level, log_drop, log_accept;

	i=0;
	
	if (nvram_match("wl_chan_list", "")) {
		for(i=0;i<14;i++)
		{
			channels[i] = i+1;
			channels[i+1] = -1;
		}	
	}
	else {
		foreach(var, nvram_safe_get("wl_chan_list"), next) {
			channels[i]=atoi(var);
			channels[i+1]=-1;
			i++;
		}
	}
	return i;
}

int
get_wl_channels()
{
	int i, channels[32];

	if (!channels_in_country(nvram_safe_get("wl_country_code"), channels))
	{
		channels_in_country_asus(nvram_safe_get("wl_country_code"), channels);
	}

	i = 0;
	while(channels[i]!=-1)
		i++;
		
	return i-1;
}

int parseXMLtext(char *dst, const char *src)
{
	char tmpstr[6];
	int len=0;
	int i=0;
	int dstlen=0;

	memset(dst, 0, sizeof(dst));
	len=strlen(src);

	for( i=0; i<len; i++ )
	{
		if ( (len-i)>=4 )
		{
			memset(tmpstr, 0, sizeof(tmpstr));
			memcpy(tmpstr, src+i, 4);
			if ( memcmp(tmpstr, "&gt;", 4)==0 )	// '>'
			{
				memset(dst+dstlen, 62, 1);
				dstlen++;
				i+=3;
				continue;
			}
			else if ( memcmp(tmpstr, "&lt;", 4)==0 )	// '<'
			{
				memset(dst+dstlen, 60, 1);
				dstlen++;
				i+=3;
				continue;
			}
		}
		if ( (len-i)>=5 )
		{
			memset(tmpstr, 0, sizeof(tmpstr));
			memcpy(tmpstr, src+i, 5);
			if ( memcmp(tmpstr, "&amp;", 5)==0 )	// '&'
			{
				memset(dst+dstlen, 38, 1);
				dstlen++;
				i+=4;
				continue;
			}
		}
		memset(dst+dstlen, src[i], 1);
		dstlen++;
	}
	dst[dstlen]=0;
	return dstlen;
}

#define TYPE_configId 1
#define TYPE_configAuthorId 2
#define TYPE_configAuthor 3
#define TYPE_ssid 4
#define TYPE_connectionType 5
#define TYPE_authentication 6
#define TYPE_encryption 7
#define TYPE_networkKey 8
#define TYPE_keyProvidedAutomatically 9
#define TYPE_ieee802Dot1xEnabled 10
#define TYPE_eapMethod 11
#define TYPE_channel2Dot4 12
#define TYPE_channel5Dot0 13
#define TYPE_deviceMode 14
#define TYPE_timeToLive 15
#define TYPE_configHash 16
#define TYPE_keyIndex 17
#if 1
void
wfc_parser(const char *run_path)
{
//	if (nvram_invmatch("wcn_enable_x", "1"))
//		return;

	if (strcmp(run_path, "")==0)
		return;

	char *p_Value = NULL;
	int p_ValueLength = 0;
	int bufferLength = 0;
	char wfc_path[128];
	memset(wfc_path, 0, sizeof(wfc_path));
	sprintf(wfc_path, "%s/SMRTNTKY/WSETTING.WFC", run_path);

	if (check_if_file_exist(wfc_path)==0) {
		return;
	}

	char *buffer = ILibReadFileFromDisk(wfc_path, &bufferLength);
	int offset = 0;
	struct ILibXMLNode *xnode = ILibParseXML(buffer,offset,bufferLength);
	struct ILibXMLNode *root = xnode;
	int i=0;
	int t_configId=0;
	int t_configAuthorId=0;
	int t_configAuthor=0;
	int t_ssid=0;
	int t_connectionType=0;
	int t_authentication=0;
	int t_encryption=0;
	int t_networkKey=0;
	int t_keyProvidedAutomatically=0;
	int t_ieee802Dot1xEnabled=0;
	int t_eapMethod=0;
	int t_channel2Dot4=0;
	int t_channel5Dot0=0;
	int t_deviceMode=0;
	int t_timeToLive=0;
	int t_configHash=0;
	int t_keyIndex=0;
	int t_primaryProfile=0;
	int t_optionalProfile=0;
	int elementType=0;
	char str_configError[256+1];
	char str_configId[36+1];
	char str_configAuthorId[36+1];
	char str_configAuthor[128+1];
	char str_ssid[32*5+1];
	char str_connectionType[4+1];
	char str_authentication[8+1];
	char str_encryption[4+1];
	char str_networkKey[63*5+1];
	char str_keyProvidedAutomatically[1+1];
	char str_ieee802Dot1xEnabled[1+1];
	char str_eapMethod[17+1];
	char str_channel2Dot4[2+1];
	char str_channel5Dot0[3+1];
	char str_deviceMode[14+1];
	char str_timeToLive[5+1];
	char str_configHash[20+1];
	char str_keyIndex[1+1];
	long int long_temp=0;
	char **endptr=NULL;
	char tmpstr[3];
	char tmpstr2[27];
	char tmpstr3[32*3+1];
	int tmp=0;
	int tmp2=0;
	int wep_type=0;
	FILE *fp=NULL;
	char MACpostfix[8+1];
	char deviceCFG_path[64];
	int reboot_wcn=0;

	memset(str_configError, 0, sizeof(str_configError));
	memset(str_configId, 0, sizeof(str_configId));
	memset(str_configAuthorId, 0, sizeof(str_configAuthorId));
	memset(str_configAuthor, 0, sizeof(str_configAuthor));
	memset(str_ssid, 0, sizeof(str_ssid));
	memset(str_connectionType, 0, sizeof(str_connectionType));
	memset(str_authentication, 0, sizeof(str_authentication));
	memset(str_encryption, 0, sizeof(str_encryption));
	memset(str_networkKey, 0, sizeof(str_networkKey));
	memset(str_keyProvidedAutomatically, 0, sizeof(str_keyProvidedAutomatically));
	memset(str_ieee802Dot1xEnabled, 0, sizeof(str_ieee802Dot1xEnabled));
	memset(str_eapMethod, 0, sizeof(str_eapMethod));
	memset(str_channel2Dot4, 0, sizeof(str_channel2Dot4));
	memset(str_channel5Dot0, 0, sizeof(str_channel5Dot0));
	memset(str_deviceMode, 0, sizeof(str_deviceMode));
	memset(str_timeToLive, 0, sizeof(str_timeToLive));
	memset(str_configHash, 0, sizeof(str_configHash));
	memset(str_keyIndex, 0, sizeof(str_keyIndex));

	if(ILibProcessXMLNodeList(root)!=0)
	{
		/* The XML is not well formed! */
		strcpy(str_configError, "Invalid WCN-UFD XML");
		printf("%s\n", str_configError);
		logmessage("WCN-UFD", str_configError);
		goto end1;
	}

	while(xnode!=NULL)
	{
		if( xnode->StartTag!=0 )
		{
			if(xnode->NameLength==8 && memcmp("configId", xnode->Name, xnode->NameLength)==0)
			// type="fc:str36"
			{
				elementType=TYPE_configId;
				if (++t_configId>1)
					goto end2;
			}
			else if(xnode->NameLength==14 && memcmp("configAuthorId", xnode->Name, xnode->NameLength)==0)
			// type="fc:str36"
			{
				elementType=TYPE_configAuthorId;
				if (++t_configAuthorId>1)
					goto end2;
			}
			else if(xnode->NameLength==12 && memcmp("configAuthor", xnode->Name, xnode->NameLength)==0)
			// type="fc:strMax128"
			{
				elementType=TYPE_configAuthor;
				if (++t_configAuthor>1)
					goto end2;
			}
			else if(xnode->NameLength==4 && memcmp("ssid", xnode->Name, xnode->NameLength)==0)
			// type="fc:strMax32"
			{
				elementType=TYPE_ssid;
				if (++t_ssid>1)
					goto end2;
			}
			else if(xnode->NameLength==14 && memcmp("connectionType", xnode->Name, xnode->NameLength)==0)
			// base="xs:string"
			{
				elementType=TYPE_connectionType;
				if (++t_connectionType>1)
					goto end2;
			}
			else if(xnode->NameLength==14 && memcmp("authentication", xnode->Name, xnode->NameLength)==0)
			// base="xs:string"
			{
				elementType=TYPE_authentication;
				if (++t_authentication>1)
					goto end2;
			}
			else if(xnode->NameLength==10 && memcmp("encryption", xnode->Name, xnode->NameLength)==0)
			// base="xs:string"
			{
				elementType=TYPE_encryption;
				if (++t_encryption>1)
					goto end2;
			}
			else if(xnode->NameLength==10 && memcmp("networkKey", xnode->Name, xnode->NameLength)==0)
			// type="fc:strMin5Max64"
			{
				elementType=TYPE_networkKey;
				if (++t_networkKey>1)
					goto end2;
			}
			else if(xnode->NameLength==24 && memcmp("keyProvidedAutomatically", xnode->Name, xnode->NameLength)==0)
			// type="xs:boolean"
			{
				elementType=TYPE_keyProvidedAutomatically;
				if (++t_keyProvidedAutomatically>1)
					goto end2;
			}
			else if(xnode->NameLength==19 && memcmp("ieee802Dot1xEnabled", xnode->Name, xnode->NameLength)==0)
			// type="xs:boolean"
			{
				elementType=TYPE_ieee802Dot1xEnabled;
				if (++t_ieee802Dot1xEnabled>1)
					goto end2;
			}
			/*
			The following elements are included in the schema but are not supported in the Windows XP SP2 release of the wizard.
			*/
			else if(xnode->NameLength==9 && memcmp("eapMethod", xnode->Name, xnode->NameLength)==0)
			// base="xs:string"
			{
				elementType=TYPE_eapMethod;
				if (++t_eapMethod>1)
					goto end2;
			}
			else if(xnode->NameLength==12 && memcmp("channel2Dot4", xnode->Name, xnode->NameLength)==0)
			// type="xs:positiveInteger"
			{
				elementType=TYPE_channel2Dot4;
				if (++t_channel2Dot4>1)
					goto end2;
			}
			else if(xnode->NameLength==12 && memcmp("channel5Dot0", xnode->Name, xnode->NameLength)==0)
			// type="xs:positiveInteger"
			{
				elementType=TYPE_channel5Dot0;
				if (++t_channel5Dot0>1)
					goto end2;
			}
			else if(xnode->NameLength==10 && memcmp("deviceMode", xnode->Name, xnode->NameLength)==0)
			// base="xs:string"
			{
				elementType=TYPE_deviceMode;
				if (++t_deviceMode>1)
					goto end2;
			}
			else if(xnode->NameLength==10 && memcmp("timeToLive", xnode->Name, xnode->NameLength)==0)
			// type="xs:positiveInteger"
			{
				elementType=TYPE_timeToLive;
				if (++t_timeToLive>1)
					goto end2;
			}
			else if(xnode->NameLength==10 && memcmp("configHash", xnode->Name, xnode->NameLength)==0)
			// type="fc:str20"
			{
				elementType=TYPE_configHash;
				if (++t_configHash>1)
					goto end2;
			}
			else if(xnode->NameLength==8 && memcmp("keyIndex", xnode->Name, xnode->NameLength)==0)
			// type="xs:positiveInteger"
			{
				elementType=TYPE_keyIndex;
				if (++t_keyIndex>1)
					goto end2;
			}
			else if(xnode->NameLength==14 && memcmp("primaryProfile", xnode->Name, xnode->NameLength)==0)
			// type="fc:profileInstance"
			{
				if (++t_primaryProfile>1)
					goto end2;
				else
					goto next_loop;
			}
			else if(xnode->NameLength==15 && memcmp("optionalProfile", xnode->Name, xnode->NameLength)==0)
			// type="fc:profileInstance"
			{
				if (++t_optionalProfile>32)
					goto end2;
				else
					goto next_loop;
			}
			else
			{
				goto next_loop;
			}

//			printf("%d\n", ++i);
			xnode->Name[xnode->NameLength]=0;
			printf("Name: %s\n", xnode->Name);
			p_ValueLength = ILibReadInnerXML(xnode,&p_Value);
			p_Value[p_ValueLength]=0;
			printf("Value %s\n\n", p_Value);

			if (elementType==TYPE_configId)
			{
				if (p_ValueLength!=36)
					goto end2;

   				for( i=0; i<p_ValueLength; i++ )
         				if( p_Value[i]<' ' || p_Value[i] >'~') 
         					goto end2;

				if (strcasecmp(nvram_safe_get("wl_configId"), p_Value)==0)
				{
					printf("have been configured using this WCN-UFD configuration file already\n");
					logmessage("WCN-UFD", "have been configured using this WCN-UFD configuration file already");
					goto end0;
				}

				strcpy(str_configId, p_Value);
			}
			else if (elementType==TYPE_configAuthorId)
			{
				if (p_ValueLength!=36)
					goto end2;

   				for( i=0; i<p_ValueLength; i++ )
         				if( p_Value[i]<' ' || p_Value[i] >'~') 
         					goto end2;

				strcpy(str_configAuthorId, p_Value);
			}
			else if (elementType==TYPE_configAuthor)
			{
				if (p_ValueLength>128 || p_ValueLength==0)
					goto end2;

   				for( i=0; i<p_ValueLength; i++ )
         				if( p_Value[i]<' ' || p_Value[i] >'~') 
         					goto end2;

				strcpy(str_configAuthor, p_Value);
			}
			else if (elementType==TYPE_ssid)
			{
				if (p_ValueLength>32*5 || p_ValueLength==0)
					goto end2;

   				for( i=0; i<p_ValueLength; i++ )
         				if( p_Value[i]<' ' || p_Value[i] >'~') 
         					goto end2;

				tmp=parseXMLtext(tmpstr3, p_Value);
				if (tmp>32 || tmp==0)
					goto end2;

				strcpy(str_ssid, tmpstr3);
			}

			else if (elementType==TYPE_connectionType)
			{
				if (p_ValueLength==4 && strncasecmp("IBSS", p_Value, 4)==0)
					strcpy(str_connectionType, "IBSS");
				else if (p_ValueLength==3 && strncasecmp("ESS", p_Value, 3)==0)
					strcpy(str_connectionType, "ESS");
				else
					goto end2;
			}
			else if (elementType==TYPE_authentication)
			{
				if (p_ValueLength==4 && strncasecmp("open", p_Value, 4)==0)
					strcpy(str_authentication, "open");
				else if (p_ValueLength==6 && strncasecmp("shared", p_Value, 6)==0)
					strcpy(str_authentication, "shared");
				else if (p_ValueLength==8 && strncasecmp("WPA-NONE", p_Value, 8)==0)
				{
//					strcpy(str_authentication, "WPA-NONE");
					printf("aaa\n");
					goto end3;
				}
				else if (p_ValueLength==3 && strncasecmp("WPA", p_Value, 3)==0)
					strcpy(str_authentication, "WPA");
				else if (p_ValueLength==6 && strncasecmp("WPAPSK", p_Value, 6)==0)
					strcpy(str_authentication, "WPAPSK");
				else if (p_ValueLength==4 && strncasecmp("WPA2", p_Value, 4)==0)
					strcpy(str_authentication, "WPA2");
				else if (p_ValueLength==7 && strncasecmp("WPA2PSK", p_Value, 7)==0)
					strcpy(str_authentication, "WPA2PSK");
				else
					goto end2;
			}
			else if (elementType==TYPE_encryption)
			{
				if (	p_ValueLength!=3 &&
					p_ValueLength!=4
				   )
					goto end2;
					
				if (p_ValueLength==4 && strncasecmp("none", p_Value, 4)==0)
					strcpy(str_encryption, "none");
				else if (p_ValueLength==3 && strncasecmp("WEP", p_Value, 3)==0)
					strcpy(str_encryption, "WEP");
				else if (p_ValueLength==4 && strncasecmp("TKIP", p_Value, 4)==0)
					strcpy(str_encryption, "TKIP");
				else if (p_ValueLength==3 && strncasecmp("AES", p_Value, 3)==0)
					strcpy(str_encryption, "AES");
				else
					goto end2;
			}
			else if (elementType==TYPE_networkKey)
			{
				if (p_ValueLength>63*5 || (p_ValueLength<5 && p_ValueLength!=0))
					goto end2;

   				for( i=0; i<p_ValueLength; i++ )
         				if( p_Value[i]<' ' || p_Value[i] >'~') 
         					goto end2;

				tmp=parseXMLtext(tmpstr3, p_Value);
				if (tmp>64 || (tmp<5 && tmp!=0))
					goto end2;

				strcpy(str_networkKey, tmpstr3);
			}
			else if (elementType==TYPE_keyProvidedAutomatically)
			{
				if (p_ValueLength==1 && strncasecmp("0", p_Value, 1)==0)
					strcpy(str_keyProvidedAutomatically, "0");
				else if (p_ValueLength==1 && strncasecmp("1", p_Value, 1)==0)
					strcpy(str_keyProvidedAutomatically, "1");
				else
					goto end2;
			}
			else if (elementType==TYPE_ieee802Dot1xEnabled)
			{
				if (p_ValueLength==1 && strncasecmp("0", p_Value, 1)==0)
					strcpy(str_ieee802Dot1xEnabled, "0");
				else if (p_ValueLength==1 && strncasecmp("1", p_Value, 1)==0)
					strcpy(str_ieee802Dot1xEnabled, "1");
				else
					goto end2;
			}
			else if (elementType==TYPE_eapMethod)
			{
				if (p_ValueLength==7 && strncasecmp("EAP-TLS", p_Value, 7)==0)
					strcpy(str_eapMethod, "EAP-TLS");
				else if (p_ValueLength==12 && strncasecmp("PEAP-EAP-TLS", p_Value, 12)==0)
					strcpy(str_eapMethod, "PEAP-EAP-TLS");
				else if (p_ValueLength==17 && strncasecmp("PEAP-EAP-MSCHAPv2", p_Value, 17)==0)
					strcpy(str_eapMethod, "PEAP-EAP-MSCHAPv2");
				else
					goto end2;					
			}
			else if (elementType==TYPE_channel2Dot4)
			{		
				if (p_ValueLength==1 && strncasecmp("0", p_Value, 1)==0)
					strcpy(str_channel2Dot4, "0");
				else if (p_ValueLength==1 && strncasecmp("1", p_Value, 1)==0)
					strcpy(str_channel2Dot4, "1");
				else if (p_ValueLength==1 && strncasecmp("2", p_Value, 1)==0)
					strcpy(str_channel2Dot4, "2");
				else if (p_ValueLength==1 && strncasecmp("3", p_Value, 1)==0)
					strcpy(str_channel2Dot4, "3");
				else if (p_ValueLength==1 && strncasecmp("4", p_Value, 1)==0)
					strcpy(str_channel2Dot4, "4");
				else if (p_ValueLength==1 && strncasecmp("5", p_Value, 1)==0)
					strcpy(str_channel2Dot4, "5");
				else if (p_ValueLength==1 && strncasecmp("6", p_Value, 1)==0)
					strcpy(str_channel2Dot4, "6");
				else if (p_ValueLength==1 && strncasecmp("7", p_Value, 1)==0)
					strcpy(str_channel2Dot4, "7");
				else if (p_ValueLength==1 && strncasecmp("8", p_Value, 1)==0)
					strcpy(str_channel2Dot4, "8");
				else if (p_ValueLength==1 && strncasecmp("9", p_Value, 1)==0)
					strcpy(str_channel2Dot4, "9");
				else if (p_ValueLength==2 && strncasecmp("10", p_Value, 2)==0)
					strcpy(str_channel2Dot4, "10");
				else if (p_ValueLength==2 && strncasecmp("11", p_Value, 2)==0)
					strcpy(str_channel2Dot4, "11");
				else if (p_ValueLength==2 && strncasecmp("12", p_Value, 2)==0)
					strcpy(str_channel2Dot4, "12");
				else if (p_ValueLength==2 && strncasecmp("13", p_Value, 2)==0)
					strcpy(str_channel2Dot4, "13");
				else if (p_ValueLength==2 && strncasecmp("14", p_Value, 2)==0)
					strcpy(str_channel2Dot4, "14");
				else
					goto end2;
			}
			else if (elementType==TYPE_channel5Dot0)
			{
				if (p_ValueLength==1 && strncasecmp("0", p_Value, 1)==0)
					strcpy(str_channel5Dot0, "0");
				else if (p_ValueLength==2 && strncasecmp("36", p_Value, 2)==0)
					strcpy(str_channel5Dot0, "36");
				else if (p_ValueLength==2 && strncasecmp("40", p_Value, 2)==0)
					strcpy(str_channel5Dot0, "40");
				else if (p_ValueLength==2 && strncasecmp("44", p_Value, 2)==0)
					strcpy(str_channel5Dot0, "44");
				else if (p_ValueLength==2 && strncasecmp("48", p_Value, 2)==0)
					strcpy(str_channel5Dot0, "48");
				else if (p_ValueLength==2 && strncasecmp("52", p_Value, 2)==0)
					strcpy(str_channel5Dot0, "52");
				else if (p_ValueLength==2 && strncasecmp("56", p_Value, 2)==0)
					strcpy(str_channel5Dot0, "56");
				else if (p_ValueLength==2 && strncasecmp("60", p_Value, 2)==0)
					strcpy(str_channel5Dot0, "60");
				else if (p_ValueLength==3 && strncasecmp("149", p_Value, 3)==0)
					strcpy(str_channel5Dot0, "149");
				else if (p_ValueLength==3 && strncasecmp("153", p_Value, 3)==0)
					strcpy(str_channel5Dot0, "153");
				else if (p_ValueLength==3 && strncasecmp("157", p_Value, 3)==0)
					strcpy(str_channel5Dot0, "157");
				else if (p_ValueLength==3 && strncasecmp("161", p_Value, 3)==0)
					strcpy(str_channel5Dot0, "161");
				else
					goto end2;
			}
			else if (elementType==TYPE_deviceMode)
			{
				if (p_ValueLength==14 && strncasecmp("infrastructure", p_Value, 14)==0)
					strcpy(str_deviceMode, "infrastructure");
				else if (p_ValueLength==6 && strncasecmp("bridge", p_Value, 6)==0)
					strcpy(str_deviceMode, "bridge");
				else if (p_ValueLength==8 && strncasecmp("repeater", p_Value, 8)==0)
					strcpy(str_deviceMode, "repeater");
				else if (p_ValueLength==7 && strncasecmp("station", p_Value, 7)==0)
					strcpy(str_deviceMode, "station");
				else
					goto end2;
			}
			else if (elementType==TYPE_timeToLive)
			{
				if (p_ValueLength>5 || p_ValueLength==0)
					goto end2;

				long_temp=strtol(p_Value, endptr, 10);
				if (	(long_temp>(long int)86400) ||
					(long_temp<(long int)0) ||
					(long_temp==LONG_MAX) ||
					(long_temp==LONG_MIN) ||
					(endptr!=NULL)
				   )
					goto end2;
				else
					sprintf(str_timeToLive, "%d", (int)long_temp);
			}
			else if (elementType==TYPE_configHash)
			{
				if (p_ValueLength!=20)
					goto end2;

   				for( i=0; i<p_ValueLength; i++ )
         				if(	p_Value[i]<'0' ||
         					(p_Value[i] >'9' && p_Value[i]<'A') ||
         					(p_Value[i]>'F' && p_Value[i]<'a') ||
         					p_Value[i] >'f'
         				  )
         					goto end2;

				strcpy(str_configHash, p_Value);
			}
			else if (elementType==TYPE_keyIndex)
			{
				if (p_ValueLength!=1)
					goto end2;

				if (p_ValueLength==1 && strncasecmp("1", p_Value, 1)==0)
					strcpy(str_keyIndex, "1");
				else if (p_ValueLength==1 && strncasecmp("2", p_Value, 1)==0)
					strcpy(str_keyIndex, "2");
				else if (p_ValueLength==1 && strncasecmp("3", p_Value, 1)==0)
					strcpy(str_keyIndex, "3");
				else if (p_ValueLength==1 && strncasecmp("4", p_Value, 1)==0)
					strcpy(str_keyIndex, "4");
				else
					goto end2;
			}
			else
				goto end2;
		}
next_loop:
//		xnode = xnode->Peer;
		xnode = xnode->Next;
	}

	if(	t_configId==0 ||
		t_configAuthorId==0 ||
		t_configAuthor==0 ||
		t_ssid==0 ||
		t_connectionType==0 ||
		t_authentication==0 ||
		t_encryption==0 ||
		t_networkKey==0 ||
		t_keyProvidedAutomatically==0 ||
		t_ieee802Dot1xEnabled==0 ||
		t_primaryProfile==0
	)
	{
		strcpy(str_configError, "lack essential WCN-UFD XML element");
		printf("%s\n", str_configError);
		logmessage("WCN-UFD", str_configError);
		goto end1;
	}

	if (t_channel5Dot0==1 && strcmp(str_channel5Dot0, "0")!=0)
	{
		printf("xxx\n");
		goto end3;
	}

	if (atoi(str_channel2Dot4) > get_wl_channels())
	{
		printf("yyy\n");
		goto end3;
	}

	if (strcmp(str_encryption, "none")==0 && strcmp(str_authentication, "open")!=0)
	{
		printf("bbb\n");
		goto end3;
	}

	if (strcmp(str_keyProvidedAutomatically, "0")==0)
	{
		if (	strcmp(str_authentication, "WPA")==0 ||
			strcmp(str_authentication, "WPA2")==0
		   )
		{
			printf("ccc1\n");
			goto end3;
		}
	}
	else
	{
		if (	strcmp(str_ieee802Dot1xEnabled, "1")==0 ||
			strcmp(str_encryption, "WEP")!=0
		   )
			printf("ccc2\n");
			goto end3;
	}

	if (strcmp(str_ieee802Dot1xEnabled, "1")==0)
	{
		if (	strcmp(str_encryption, "WEP")!=0 ||
			(strcmp(str_encryption, "WEP")==0 && (strcmp(str_keyIndex, "1")==0 || strcmp(str_keyIndex, "4")==0)) ||
			(strcmp(str_authentication, "WPA")!=0 && strcmp(str_authentication, "WPA2")!=0)
		   )
		{
			printf("ddd\n");
			goto end3;
		}
	}

	if (strcmp(str_encryption, "none")==0 || strcmp(str_encryption, "WEP")==0)
	{
		if (	strcmp(str_authentication, "WPAPSK")==0 ||
			strcmp(str_authentication, "WPA2PSK")==0 ||
			strcmp(str_authentication, "WPA")==0 ||
			strcmp(str_authentication, "WPA2")==0
		   )
		{
			printf("eee\n");
			goto end3;
		}
	}

	if (strcmp(str_encryption, "TKIP")==0 || strcmp(str_encryption, "AES")==0)
	{
		if (	strcmp(str_authentication, "open")==0 ||
			strcmp(str_authentication, "shared")==0 ||
			strcmp(str_ieee802Dot1xEnabled, "1")==0
		   )
		{
			printf("fff\n");
			goto end3;
		}
	}
	
	if (	//(strcmp(str_authentication, "WPAPSK")==0 && strcmp(str_encryption, "AES")==0) ||
		//(strcmp(str_authentication, "WPA2PSK")==0 && strcmp(str_encryption, "TKIP")==0) ||
		(strcmp(str_authentication, "WPA")==0 && strcmp(str_encryption, "AES")==0) ||
		(strcmp(str_authentication, "WPA2")==0 && strcmp(str_encryption, "TKIP")==0)
	   )
	{
		printf("ggg\n");
		goto end3;
	}

	if (	strcmp(str_authentication, "shared")==0 ||
		strcmp(str_authentication, "WPAPSK")==0 ||
		strcmp(str_authentication, "WPA2PSK")==0 ||
		strcmp(str_authentication, "WPA")==0 ||
		strcmp(str_authentication, "WPA2")==0 ||
		strcmp(str_ieee802Dot1xEnabled, "1")==0 ||
		(strcmp(str_authentication, "open")==0 && strcmp(str_encryption, "WEP")==0)
	   )
	{
		if (strlen(str_networkKey)==0)
		{
			printf("no network key\n");
			logmessage("WCN-UFD", "no network key");
			goto end1;
		}
	}

	if (	(strcmp(str_authentication, "open")==0 && strcmp(str_encryption, "WEP")==0) ||
		strcmp(str_authentication, "shared")==0 ||
		strcmp(str_ieee802Dot1xEnabled, "1")==0
	   )
	{		
		if (	strlen(str_networkKey)==10 ||
			strlen(str_networkKey)==26
		   )
		{
			for( i=0; i<strlen(str_networkKey); i++ )
         			if (	str_networkKey[i]<'0' ||
         				(str_networkKey[i]>'9' && str_networkKey[i]<'A') ||
         				(str_networkKey[i]>'F' && str_networkKey[i]<'a') ||
         				str_networkKey[i]>'f'
         			   )
				{
					strcpy(str_configError, "invalid wireless network key");
					printf("%s\n", str_configError);
					logmessage("WCN-UFD", str_configError);

					goto end1;
				}
			for( i=0; i<strlen(str_networkKey); i++ )
				str_networkKey[i]=toupper(str_networkKey[i]);
		}
		else if (	strlen(str_networkKey)==5 ||
				strlen(str_networkKey)==13
			)
		{
			tmp=strlen(str_networkKey)*2;
			memset(tmpstr2, 0, 27);
			for( i=0; i<strlen(str_networkKey); i++ )
			{
				memset(tmpstr, 0, 3);
				sprintf(tmpstr, "%X", str_networkKey[i]);
				memcpy(&tmpstr2[2*i], tmpstr, 2);
			}
			memcpy(str_networkKey, tmpstr2, 2*tmp);
		}
		else
		{
			strcpy(str_configError, "invalid wireless network key length");
			printf("%s\n", str_configError);
			logmessage("WCN-UFD", str_configError);
			goto end1;
		}
	}
	else if (	strcmp(str_authentication, "WPAPSK")==0 ||
			strcmp(str_authentication, "WPA2PSK")==0
		)
	{
		if (strlen(str_networkKey)==64)
		{
			for( i=0; i<strlen(str_networkKey); i++ )
         			if (	str_networkKey[i]<'0' ||
         				(str_networkKey[i]>'9' && str_networkKey[i]<'A') ||
         				(str_networkKey[i]>'F' && str_networkKey[i]<'a') ||
         				str_networkKey[i]>'f'
         			   )
				{
					strcpy(str_configError, "invalid wireless network key");
					printf("%s\n", str_configError);
					logmessage("WCN-UFD", str_configError);

					goto end1;
				}
			for( i=0; i<strlen(str_networkKey); i++ )
				str_networkKey[i]=toupper(str_networkKey[i]);

/*
			for( i=0; i<64; i+=2 )
			{
				memset(tmpstr, 0, sizeof(tmpstr));
				tmpstr[0]=str_networkKey[i];
				tmp=(int)strtol(tmpstr, endptr, 16);

				memset(tmpstr, 0, sizeof(tmpstr));
				tmpstr[0]=str_networkKey[i+1];
				tmp2=(int)strtol(tmpstr, endptr, 16);

				tmp2=tmp*16+tmp2;
				str_networkKey[i/2]=tmp2;
			}
			str_networkKey[32]=0;
*/
		}
	}

	printf("%d, %s\n", t_configId, str_configId);
	printf("%d, %s\n", t_configAuthorId, str_configAuthorId);
	printf("%d, %s\n", t_configAuthor, str_configAuthor);
	printf("%d, %s\n", t_ssid, str_ssid);
	printf("%d, %s\n", t_connectionType, str_connectionType);
	printf("%d, %s\n", t_authentication, str_authentication);
	printf("%d, %s\n", t_encryption, str_encryption);
	printf("%d, %s\n", t_networkKey, str_networkKey);
	printf("%d, %s\n", t_keyProvidedAutomatically, str_keyProvidedAutomatically);
	printf("%d, %s\n", t_ieee802Dot1xEnabled, str_ieee802Dot1xEnabled);
	printf("%d\n", t_primaryProfile);
	printf("***************************************\n");
	printf("*** following elements are optional ***\n");
	printf("%d, %s\n", t_eapMethod, str_eapMethod);
	printf("%d, %s\n", t_channel2Dot4, str_channel2Dot4);
	printf("%d, %s\n", t_channel5Dot0, str_channel5Dot0);
	printf("%d, %s\n", t_deviceMode, str_deviceMode);
	printf("%d, %s\n", t_timeToLive, str_timeToLive);
	printf("%d, %s\n", t_configHash, str_configHash);
	printf("%d, %s\n", t_keyIndex, str_keyIndex);
	printf("%d\n\n", t_optionalProfile);

	nvram_set("wl_configId", str_configId);
	nvram_set("wl_ssid", str_ssid);
	/*memset(tmpstr3, 0, sizeof(tmpstr3));
	char_to_ascii(tmpstr3, str_ssid);
	nvram_set("wl_ssid2", tmpstr3);//*/

	if (t_channel2Dot4==1)
	{
		nvram_set("wl_channel", str_channel2Dot4);
	}

	if (strcmp(str_ieee802Dot1xEnabled, "1")==0)
	{
		nvram_set("wl_auth_mode", "radius");
	}
	else if (strcmp(str_authentication, "open")==0)
	{
		nvram_set("wl_auth_mode", "open");
	}
	else if (strcmp(str_authentication, "shared")==0)
	{
		nvram_set("wl_auth_mode", "shared");
	}
	else if (strcmp(str_authentication, "WPAPSK")==0
			|| strcmp(str_authentication, "WPA2PSK")==0)
	{
		nvram_set("wl_auth_mode", "psk");
// 2008.07 James. {
		/*if (strcmp(str_authentication, "WPAPSK")==0)
			nvram_set("wl_wpa_mode", "1");
		else
			nvram_set("wl_wpa_mode", "2");
		nvram_set("wl_crypto", "tkip+aes");//*/
		if (strcmp(str_authentication, "WPAPSK")==0){
			nvram_set("wl_wpa_mode", "1");
			nvram_set("wl_crypto", "tkip");
		}
		else{
			nvram_set("wl_wpa_mode", "2");
			nvram_set("wl_crypto", "aes");
		}
// 2008.07 James. }
/*
		if (strcmp(str_encryption, "TKIP")==0)
			nvram_set("wl_crypto", "tkip");
		else if (strcmp(str_encryption, "AES")==0)
			nvram_set("wl_crypto", "aes");
*/
	}
	else if (strcmp(str_authentication, "WPA")==0)
	{
		nvram_set("wl_auth_mode", "wpa");
		nvram_set("wl_crypto", "tkip");
	}
	else if (strcmp(str_authentication, "WPA2")==0)
	{
		nvram_set("wl_auth_mode", "wpa2");
		nvram_set("wl_crypto", "aes");
	}

	if (strcmp(str_encryption, "TKIP")==0 || strcmp(str_encryption, "AES")==0)
	{
		nvram_set("wl_wpa_psk", str_networkKey);
		nvram_set("wl_wep_x", "0");
	}
	else if (strcmp(str_encryption, "WEP")==0)
	{
		if (strlen(str_networkKey)==10)
		{
			nvram_set("wl_wep_x", "1");
			nvram_set("wl_key1", "6565656565");
			nvram_set("wl_key2", "8383838383");
			nvram_set("wl_key3", "8585858585");
			nvram_set("wl_key4", "8383838383");
		}
		else if (strlen(str_networkKey)==26)
		{
			nvram_set("wl_wep_x", "2");
			nvram_set("wl_key1", "65656565656565656565656565");
			nvram_set("wl_key2", "83838383838383838383838383");
			nvram_set("wl_key3", "85858585858585858585858585");
			nvram_set("wl_key4", "83838383838383838383838383");
		}

		if (t_keyIndex==0)
		{
			nvram_set("wl_key", "1");
			nvram_set("wl_key1", str_networkKey);
		}
		else
		{
			nvram_set("wl_key", str_keyIndex);
			memset(tmpstr2, 0, sizeof(tmpstr2));
			sprintf(tmpstr2, "wl_key%s", str_keyIndex);
			nvram_set(tmpstr2, str_networkKey);
		}
	}
	else
	{
		nvram_set("wl_wep_x", "0");
		nvram_set("wl_key1", "");
		nvram_set("wl_key2", "");
		nvram_set("wl_key3", "");
		nvram_set("wl_key4", "");
		nvram_set("wl_wpa_psk", "");
	}

	if (t_timeToLive==1)
	{
		nvram_set("wl_wpa_gtk_rekey", str_timeToLive);
	}
	
	reboot_wcn=1;

	goto end1;
end3:
	strcpy(str_configError, "unsupported WCN-UFD XML element value");
	printf("%s\n", str_configError);
	logmessage("WCN-UFD", str_configError);
	goto end1;
end2:
	strcpy(str_configError, "Invalid WCN-UFD XML element");
	printf("%s\n", str_configError);
	logmessage("WCN-UFD", str_configError);
end1:
	memset(MACpostfix, 0, sizeof(MACpostfix));
	memset(deviceCFG_path, 0, sizeof(deviceCFG_path));
	memset(tmpstr2, 0, sizeof(tmpstr2));
	strcpy(tmpstr2, nvram_safe_get("et0macaddr")+6);
	tmpstr2[2]=0;
	tmpstr2[5]=0;
	tmpstr2[8]=0;
	sprintf(MACpostfix, "%s%s%s%s", tmpstr2, tmpstr2+3, tmpstr2+6, tmpstr2+9);
	for( i=0; i<strlen(MACpostfix); i++ )
		MACpostfix[i]=toupper(MACpostfix[i]);
	sprintf(deviceCFG_path, "%s/SMRTNTKY/DEVICE/%s.WFC", run_path, MACpostfix);
	fp=fopen(deviceCFG_path, "w");

	if (fp!=NULL)
	{
		fprintf(fp, "<?xml version=\"1.0\"?>\n");
		fprintf(fp, "<device xmlns=\"http://www.microsoft.com/provisioning/DeviceProfile/2004\">\n");
		fprintf(fp, "\t<configId>%s</configId>\n", str_configId);
		if (strlen(str_configError)!=0)
			fprintf(fp, "\t<configError>%s</configError>\n", str_configError);
		fprintf(fp, "\t<manufacturer>ASUSTeK</manufacturer>\n");
		fprintf(fp, "\t<manufacturerURL>http://www.asus.com/</manufacturerURL>\n");
		fprintf(fp, "\t<modelName>%s</modelName>\n", nvram_safe_get("productid"));
		memset(tmpstr2, 0, sizeof(tmpstr2));
		strcpy(tmpstr2, nvram_safe_get("et0macaddr"));
		for( i=0; i<strlen(tmpstr2); i++ )
			tmpstr2[i]=toupper(tmpstr2[i]);
		fprintf(fp, "\t<serialNumber>%s</serialNumber>\n", tmpstr2);
		fprintf(fp, "\t<firmwareVersion>%s</firmwareVersion>\n", nvram_safe_get("firmver"));
		fprintf(fp, "\t<deviceType>Access_Point</deviceType>\n");
		fprintf(fp, "</device>\n");
		fclose(fp);
	}
end0:
	ILibDestructXMLNodeList(root);
// 2007.12 James {
	//umount_disc_parts_rc(0);
	//umount_disc_parts_rc(1);
// 2007.12 James }
	
	if (reboot_wcn==1){
		nvram_set("reboot_WCN", "2");
		nvram_set("x_Setting", "1");/* Cherry Cho added in 2007/4/13. */
//		system("killall wsccmd");
		stop_wps();
	}
}

void char_to_ascii(char *output, char *input)// Transfer Char to ASCII
{                                                   // Cherry_Cho added in 2006/9/29
        int i;
        char tmp[10];
        char *ptr;
                                                                                                               
        ptr = output;
                                                                                                               
        for( i=0; i<strlen(input); i++ )
        {
                if((input[i]>='0' && input[i] <='9')
                   ||(input[i]>='A' && input[i]<='Z')
                   ||(input[i] >='a' && input[i]<='z')
                   || input[i] == '!' || input[i] == '*'
                   || input[i] == '(' || input[i] == ')'
                   || input[i] == '_' || input[i] == '-'
                   || input[i] == "'" || input[i] == '.')
                {
                        *ptr = input[i];
                        ptr ++;
                }
                else
                {
                        sprintf(tmp, "%%%.02X", input[i]);
                        strcpy(ptr, tmp);
                        ptr += 3;
                }
        }
                                                                                                               
        *ptr = '\0';
                                                                                                               
}
#endif
#endif

#ifdef LINUX26
/* Copy from fdisk  for RTAN23 AMCC automount */
FILE *fopen_or_warn(const char *path, const char *mode)// mod for RTAN23
{
        FILE *fp = fopen(path, mode);
        if (!fp) {
                printf("hotplug USB mass: No such file or directory: %s \n", path);
                errno = 0;
                return NULL;
        }
        return fp;
}
#endif

#ifdef USB_SUPPORT
/* insert usb mass storage */
int
hotplug_usb_mass(char *product)	// modified by Jiahao for WL500gP
{	
	DIR *dir_to_open, *dir_of_usb, *usb_dev_disc, *usb_dev_part;
	char usb_disc[128], usb_part[128], usb_file_part[128];
	char usb_disc_mount_path[128], usb_disc_mount_path2[128], tmp[32];
	char usb_disc_fs_path[128];
	int n=0, m=0, p=0;
	struct dirent *dp, *dp_disc, **dpopen;
	char tmpstr[128], test_path[128], test_path0[128], test_path1[128], tmpstr2[128];
	int i, j, disc_index;
	FILE *fp;
	int ret=0;
	int apps_comp=0;
	int apps_status=0;
	int apps_disk_free=0;
	int apps_comp_pre=0;
	int new_disc[2];
	new_disc[0]=0;
	new_disc[1]=0;
#ifdef DLM
	char *buf=NULL;
	int buflen=0;
	struct sysinfo info;
#endif
	char dev_real_path[PATH_MAX];	// 2007.11 James.
	
	nvram_set("usb_storage_device", "");	// 2007.12 James.
	
	dir_to_open=dir_of_usb=usb_dev_disc=usb_dev_part=NULL;
	strcpy(tmp, "");

#ifdef DLM
	nvram_set("usb_storage_busy", "1");
	nvram_set("apps_status_checked", "1");

	if (	nvram_match("eject_from_web", "1") &&
		(nvram_invmatch("usb_disc0_dev", "") || nvram_invmatch("usb_disc1_dev", ""))
	   )
		goto eject;
#endif

	if (nvram_match("usb_ftpenable_x", "0")) return 0;

	// Mount USB to system

#ifdef LINUX26

        FILE *procpt;
        char line[30], ptname[60], tempusb[20], tempusb2[20], buf2[20];
        int ma, mi, sz;
        char haveusbdisk = 0;                   //for the follow part is similar to "df"
	char repeat_part = 0;
	int repeat_disc = -1;//-1: no repeat disc, postive value is the ma value of repeat disc
	int MAX_RETRY = 5, retry_count = 0; // 2009.06 James.
	int fd=0;

	if (check_if_dir_exist("/proc/scsi/usb-storage")) {
wait:
		procpt = fopen_or_warn("/proc/partitions", "r");
		if(procpt == NULL){
			nvram_set("usb_storage_device", product);
			return -1;
		}

// 2009.06 James. {
	        char *disc0_dev = nvram_safe_get("usb_disc0_dev");
        	char *disc1_dev = nvram_safe_get("usb_disc1_dev");
        	char *disc0_nod = NULL, *disc1_nod = NULL;
 		char firstPart[8];
	        int firstPart_len = 0;
// 2009.06 James. }
		
		while (fgets(line, sizeof(line), procpt))       //The idea from "df"
		{
			//sleep(2);// for partitions ready
			if ((sscanf(line, " %d %d %d %[^\n ]", &ma, &mi, &sz, ptname)) != 4)
                                continue;
                        else {
// 2009.06 James. {
//                                strncpy(tempusb, ptname, 2);
                                if(strlen(disc0_dev) > 0)
                                        disc0_nod = disc0_dev+5;
                                else
                                        disc0_nod = "";
                                if(strlen(disc1_dev) > 0)
                                        disc1_nod = disc1_dev+5;
                                else
                                        disc1_nod = "";
printf("ptname=%s, disc0_nod=%s, disc1_nod=%s.\n", ptname, disc0_nod, disc1_nod);
                                if(!strncmp(ptname, "sd", 2) 
						&& strcmp(ptname, disc0_nod) 
						&& strcmp(ptname, disc1_nod)
						&& !isdigit(ptname[strlen(ptname)-1])
						){
                                        haveusbdisk = 1;
                                        memset(firstPart, 0, 8);
                                        sprintf(firstPart, "%s1", ptname);

                                        if(fgets(line, sizeof(line), procpt)
                                                        && sscanf(line, " %d %d %d %[^\n ]", &ma, &mi, &sz, ptname) == 4
                                                        ){
                                                firstPart_len = strlen(firstPart);
                                                if(strncmp(ptname, firstPart, firstPart_len)){
printf("There is not the first partition, so wait 2 seconds...\n");
                                                        sleep(10);
                                                }
                                        }
                                        else{
printf("The end of /proc/partitions, so wait 2 seconds...\n");
                                                sleep(10);
                                        }
// 2009.06 James. }

                                        break;
				}
			}
		} //ok
		fclose(procpt);
		
                if (!haveusbdisk && retry_count < MAX_RETRY) {
			++retry_count;
			printf("wait for the partition table ok. (retry_count=%d)\n", retry_count);
                        sleep(2);
                        goto wait;
                }
                else{
//                        usb_dev_disc = opendir("/proc/scsi/usb-storage");
                        procpt = fopen_or_warn("/proc/partitions", "r");
                        while (fgets(line, sizeof(line), procpt)) {
                                if (sscanf(line, " %d %d %d %[^\n ]", &ma, &mi, &sz, ptname) != 4)
                                        continue;
				memset(tempusb, 0, 20); // 2009.06 James.
                                strncpy(tempusb, ptname, 2);
				if (!strncmp(tempusb, "sd", 2) && ma == 8)
				{
					printf("tempusb: %s,ptname: %s:len=%d, ma=%d,mi=%d,sz=%d\n", tempusb, ptname, strlen(ptname), ma, mi, sz);
                                        if ((mi%16) == 0)       //disk
					{
                                		printf("USB Disk: [%s] found.\n", ptname);
                                        	sprintf(usb_disc, "/dev/%s", ptname);
                                                if ((nvram_match("usb_disc0_dev", usb_disc))
                                                || (nvram_match("usb_disc1_dev", usb_disc)))    
                                                {
                                                        printf("nvram setting repeat, go to next disc.\n");
                                                        repeat_disc=(mi/16);
                                                        printf("The Repeat disc mi=%d has been mounted.\n",repeat_disc);
                                                        strcpy(usb_disc,"");
                                                        continue;
                                                }
                                                else
                                                {
                                                        sprintf(test_path0, nvram_get("usb_disc0_dev"));
                                                        sprintf(test_path1, nvram_get("usb_disc1_dev"));

                                                        if (nvram_match("usb_disc0_dev", "") && strcmp(usb_disc, test_path1) != 0)
                                                        {
                                                                nvram_set("usb_disc0_dev", usb_disc);
                                                                new_disc[0] = 1;
                                                        }
                                                        else if (nvram_match("usb_disc1_dev", "") && strcmp(usb_disc, test_path0) != 0)
                                                        {
                                                                nvram_set("usb_disc1_dev", usb_disc);
                                                                new_disc[1] = 1;
                                                        }
							if (is_no_partition(ptname))    // added by Jiahao
								goto mount; //for no-partition device (APPLE iPOD shuffle)

                                                        continue;
                                                }
                                        }

                                        else if ((mi%16) != 0 && repeat_disc != (mi/16))        //partition
                                        {
                                                printf("USB Partition: [%s] found.\n", ptname);
                                                goto mount;
                                        }
                                        else
                                        {
                                                //repeat partition goto next partition.
                                                continue;
                                        }
                                }
                                else /* ma!=8 */
                                {
                                        //printf("Unknown type disk /dev/%s \n",ptname); 
                                        continue;
				}
mount:
	memset(usb_part, 0, sizeof(usb_part));
        sprintf(usb_part, "/dev/%s", ptname);
        printf("The usb_part=%s is going to check if mounted.\n",usb_part);


	/* Make devices */
	mknod(usb_part, S_IFBLK | S_IRUSR | S_IWUSR | S_IRGRP, makedev(ma, mi));

        /* check for the mounted partition */
        for(i=0;i<2;++i)
        {
                strcpy(tempusb2,"");
                printf("Check for the mounted partition.\n");
                sprintf(tempusb2, "usb_disc%d_index", i);

                if(strcmp(nvram_get(tempusb2),"0")==0)
                {
                        printf("No mounted partitions at this disc%d.\n",i);
                        continue;
                }

                disc_index=atoi(nvram_get(tempusb2));
                printf(" usb_disc%d_index=%d\n",i,disc_index);

                j=0;
                while( j < disc_index)
                {
                        strcpy(buf2,"");
                        strcpy(tempusb2,"");
                        sprintf(buf2, "usb_disc%d_fs_path%d", i, j);
                        sprintf(tempusb2,nvram_get(buf2));
                        printf("Check with the usb_disc%d_fs_path%d=%s\n",i,j,tempusb2);

                        if(strcmp(tempusb2,usb_part)==0)
                        {
                                repeat_part=1;
                                break;
                        }
                        j++;
                }
        }


        if(repeat_part)
        {
                printf("The usb_part= %s has been mounted, goto the next partition\n", usb_part);
                repeat_part=0; //reset record tag
                continue;
        }

        /* end of check for the mounted disk */

                                printf("Repeat partition checking pass, usb_part= %s\n", usb_part);
                                printf("mkdir part name n=%d\n", n);
                                if (n == 0) {
					memset(usb_file_part, 0, sizeof(usb_file_part));
                                        sprintf(usb_file_part, "/tmp/harddisk");
                                        printf("mkdir %s\n", usb_file_part);
                                        mkdir_if_none(usb_file_part);
                                }

                                sprintf(usb_file_part, "/tmp/harddisk/part%d", n);
                                while (check_if_dir_exist(usb_file_part) == 1)
                                        sprintf(usb_file_part, "/tmp/harddisk/part%d", ++n);
                                mkdir_if_none(usb_file_part);
                                printf("mkdir %s\n", usb_file_part);
#ifdef MOUNTALL
                                eval("mount", usb_part, usb_file_part);
                                printf("mountall\n");
#else /* !MOUNTALL */
                                printf("Ready to mount,\n usb_part= %s, usb_file_part= %s\n", usb_part, usb_file_part);
                                if (mount_r(usb_part, usb_file_part)) {
                                        printf("Mount ok, save parameter into nvram\n", usb_part);
                                        if (strcmp(usb_disc, nvram_safe_get("usb_disc0_dev")) == 0) {
                                                j = atoi(nvram_safe_get("usb_disc0_index"));
                                                sprintf(usb_disc_mount_path, "usb_disc0_path%d", j);
                                                sprintf(usb_disc_mount_path2, "usb_disc0_index");
                                                sprintf(usb_disc_fs_path, "usb_disc0_fs_path%d", j);
                                        } else if (strcmp(usb_disc, nvram_safe_get("usb_disc1_dev")) == 0) {
                                                j = atoi(nvram_safe_get("usb_disc1_index"));
                                                sprintf(usb_disc_mount_path, "usb_disc1_path%d", j);
                                                sprintf(usb_disc_mount_path2, "usb_disc1_index");
                                                sprintf(usb_disc_fs_path, "usb_disc1_fs_path%d", j);
                                        }

                                        nvram_set(usb_disc_mount_path, usb_file_part);
                                        nvram_set(usb_disc_fs_path, usb_part);
                                        sprintf(usb_disc_mount_path, "%d", ++j);
                                        nvram_set(usb_disc_mount_path2, usb_disc_mount_path);
                                        n++;
                                        m++;
                                        p++;
                                } else {
                                        printf("Mount %s failed\n", usb_part);
                                        rmdir(usb_file_part);
                                }
#endif  /* MOUNTALL */
                        }

                        if (m == 0 && ma == 8 && mi != 0)       // There is no other partition
                        {
                                sprintf(usb_part, "/dev/%s", ptname);


                                /* check for the mounted partition */

                                for(i=0;i<2;++i)
                                {
                                        strcpy(tempusb2,"");
                                        printf("Only one new partition, check with the mounted partition.\n");
                                        sprintf(tempusb2, "usb_disc%d_index", i);

                                        if(strcmp(nvram_get(tempusb2),"0")==0)
                                        {
                                                printf("No mounted partition at disc%d.\n",i);
                                                continue;
                                        }


                                        disc_index=atoi(nvram_get(tempusb2));
                                        for(j=0; j<disc_index; ++j)
                                        {
                                                strcpy(buf2,"");
                                                strcpy(tempusb2,"");
                                                sprintf(buf2, "usb_disc%d_fs_path%d", i, j);
                                                sprintf(tempusb2,nvram_get(buf2));
                                                printf("Check with the  usb_disc%d_fs_path%d=%s\n",i,j,tempusb2);

                                                if(strcmp(tempusb2,usb_part)==0)
                                                {
                                                        repeat_part=1;
                                                        break;
                                                }
                                        }
                                }

                                if(repeat_part)
                                {
                                        printf("The only usb_part= %s has been mounted, check the next partition\n", usb_part);
                                        repeat_part=0; //reset record tag
                                        nvram_set("usb_storage_busy", "0");
                                        printf("the only partition has been mounted, hotplug_usb_mass setting usb unbusy.\n");//zhanbx++
                                        return; // the only disk has been mounted
                                }

                                /* end of check for the mounted disk */

                                printf("End of the check the only mounted partition\n");
                                if (n == 0) {
                                        sprintf(usb_file_part, "/tmp/harddisk");
                                        mkdir_if_none(usb_file_part);
                                }
                                sprintf(usb_file_part, "/tmp/harddisk/part%d", n);
                                while (check_if_dir_exist(usb_file_part) == 1)
                                        sprintf(usb_file_part, "/tmp/harddisk/part%d", ++n);
                                mkdir_if_none(usb_file_part);
                                printf("Only one partition, ready to mount,\nusb_part= %s, usb_file_part= %s\n", usb_part, usb_file_part);

#ifdef MOUNTALL
                                eval("mount", usb_part, usb_file_part);
#else   /* !MOUNTALL */
                                printf("Start to mount 2,\nusb_part= %s, usb_file_part= %s\n", usb_part, usb_file_part);
                                if (mount_r(usb_part, usb_file_part)) {

                                        sprintf(test_path0, nvram_get("usb_disc0_dev"));
                                        sprintf(test_path1, nvram_get("usb_disc1_dev"));

                                        if (nvram_match("usb_disc0_dev", "") && strcmp(usb_disc, test_path1) != 0) {
                                                nvram_set("usb_disc0_dev", usb_disc);
                                                new_disc[0] = 1;
                                        } else if (nvram_match("usb_disc1_dev", "") && strcmp(usb_disc, test_path0) != 0) {
                                                nvram_set("usb_disc1_dev", usb_disc);
                                                new_disc[1] = 1;
                                       }

                                        if (strncmp(usb_disc, nvram_safe_get("usb_disc0_dev"), 8) == 0) {
                                                nvram_set("usb_disc0_path0", usb_file_part);
                                                nvram_set("usb_disc0_fs_path0", usb_part);
                                                nvram_set("usb_disc0_index", "1");
                                        } else if (strncmp(usb_disc, nvram_safe_get("usb_disc1_dev"), 8) == 0) {
                                                nvram_set("usb_disc1_path0", usb_file_part);
                                                nvram_set("usb_disc1_fs_path0", usb_part);
                                                nvram_set("usb_disc1_index", "1");
                                        }

                                        n++;
                                        p++;
                                } else {
                                        rmdir(usb_file_part);
                                        printf("mount %s failed\n", usb_part);
                                }
#endif  /* MOUNTALL */
                        }
                        /*printf("\n\n**********************************************\n");
                        write_disc_status();
                        printf("**********************************************\n\n");//*/
                }
        }

#else //!LINUX26
	if((usb_dev_disc = opendir("/dev/discs")))
                        if (!haveusbdisk) {
	{
		while(usb_dev_disc && (dp=readdir(usb_dev_disc)))
		{
			if(!strncmp(dp->d_name, "..", NAME_MAX) || !strncmp(dp->d_name, ".", NAME_MAX) /*|| !strncmp(dp->d_name, "disc0", NAME_MAX)*/)
				continue;

			sprintf(usb_disc, "/dev/discs/%s", dp->d_name);
// 2007.11 James {
			realpath(usb_disc, dev_real_path);
			memset(usb_disc, 0, sizeof(usb_disc));
			strcpy(usb_disc, dev_real_path);
// 2007.11 James }
			if (nvram_match("usb_disc0_dev", usb_disc))
				continue;
			if (nvram_match("usb_disc1_dev", usb_disc))
				continue;

			//logs(usb_disc);
			if((usb_dev_part = opendir(usb_disc)))
			{
				m = 0;	// part count of each disc
				while(usb_dev_part && (dp_disc=readdir(usb_dev_part)) && (m<7))
				{	
					//logs(dp_disc->d_name);
					if(	!strncmp(dp_disc->d_name, "..", NAME_MAX) ||
						!strncmp(dp_disc->d_name, ".", NAME_MAX) ||
						!strncmp(dp_disc->d_name, "disc", NAME_MAX) ||
						!strncmp(dp_disc->d_name, "generic", NAME_MAX)
					  )
						continue;

					if (m==0)
					{
						sprintf(test_path0, nvram_get("usb_disc0_dev"));
						sprintf(test_path1, nvram_get("usb_disc1_dev"));

						if (nvram_match("usb_disc0_dev", "") && strcmp(usb_disc, test_path1)!=0)
						{
							nvram_set("usb_disc0_dev", usb_disc);
							nvram_set("usb_disc0_safely_removed", "0");	// 2008.06 James.
							new_disc[0]=1;
						}
						else if (nvram_match("usb_disc1_dev", "") && strcmp(usb_disc, test_path0)!=0)
						{
							nvram_set("usb_disc1_dev", usb_disc);
							nvram_set("usb_disc1_safely_removed", "0");	// 2008.06 James.
							new_disc[1]=1;
						}
					}

					sprintf(usb_part, "/dev/discs/%s/%s", dp->d_name, dp_disc->d_name);
					realpath(usb_part, dev_real_path);	// 2007.11 James.
					if (n==0) {
						sprintf(usb_file_part, "/tmp/harddisk");
						mkdir_if_none(usb_file_part);
					}
					sprintf(usb_file_part, "/tmp/harddisk/part%d", n);
					while (check_if_dir_exist(usb_file_part)==1)
						sprintf(usb_file_part, "/tmp/harddisk/part%d", ++n);
					mkdir_if_none(usb_file_part);
#ifdef MOUNTALL
					//eval("mount", usb_part, usb_file_part);
					eval("mount", dev_real_path, usb_file_part);	// 2007.11 James.
					printf("mountall\n");
#else
					//if (mount_r(usb_part, usb_file_part))
					if(mount_r(dev_real_path, usb_file_part))	// 2007.11 James.
					{
						if (strcmp(usb_disc, nvram_safe_get("usb_disc0_dev"))==0)
						{
							j=atoi(nvram_safe_get("usb_disc0_index"));
							sprintf(usb_disc_mount_path, "usb_disc0_path%d", j);
							sprintf(usb_disc_mount_path2, "usb_disc0_index");
							sprintf(usb_disc_fs_path, "usb_disc0_fs_path%d", j);
						}
						else if (strcmp(usb_disc, nvram_safe_get("usb_disc1_dev"))==0)
						{
							j=atoi(nvram_safe_get("usb_disc1_index"));
							sprintf(usb_disc_mount_path, "usb_disc1_path%d", j);
							sprintf(usb_disc_mount_path2, "usb_disc1_index");
							sprintf(usb_disc_fs_path, "usb_disc1_fs_path%d", j);
						}
						nvram_set(usb_disc_mount_path, usb_file_part);
						//nvram_set(usb_disc_fs_path, usb_part);
						nvram_set(usb_disc_fs_path, dev_real_path);	// 2007.11 James.
						sprintf(usb_disc_mount_path, "%d", ++j);
						nvram_set(usb_disc_mount_path2, usb_disc_mount_path);
						n++;
						m++;
						p++;
					}
					else
					{
						rmdir(usb_file_part);
						//printf("mount %s failed\n", usb_part);
						printf("mount %s failed\n", dev_real_path);	// 2007.11 James.
					}
#endif	
				}

				if (!m) // There is no other partition
				{
					sprintf(usb_part, "/dev/discs/%s/disc", dp->d_name);
					realpath(usb_part, dev_real_path);	// 2007.11 James.
					if (n==0) 
					{
						sprintf(usb_file_part, "/tmp/harddisk");
						mkdir_if_none(usb_file_part);
					}
					sprintf(usb_file_part, "/tmp/harddisk/part%d", n);
					while (check_if_dir_exist(usb_file_part)==1)
						sprintf(usb_file_part, "/tmp/harddisk/part%d", ++n);
					mkdir_if_none(usb_file_part);
					
#ifdef MOUNTALL
					//eval("mount", usb_part, usb_file_part);
					eval("mount", dev_real_path, usb_file_part);	// 2007.11 James.
#else
					//if(mount_r(usb_part, usb_file_part))
					if(mount_r(dev_real_path, usb_file_part))	// 2007.11 James.
					{
						
						sprintf(test_path0, nvram_get("usb_disc0_dev"));
						sprintf(test_path1, nvram_get("usb_disc1_dev"));

						if (nvram_match("usb_disc0_dev", "") && strcmp(usb_disc, test_path1)!=0)
						{
							nvram_set("usb_disc0_dev", usb_disc);
							new_disc[0]=1;
						}
						else if (nvram_match("usb_disc1_dev", "") && strcmp(usb_disc, test_path0)!=0)
						{
							nvram_set("usb_disc1_dev", usb_disc);
							new_disc[1]=1;
						}

						//if (strncmp(usb_part, nvram_safe_get("usb_disc0_dev"), 16)==0)
						if(!strncmp(usb_part, nvram_safe_get("usb_disc0_dev"), 33))	// 2007.11 James.
						{
							nvram_set("usb_disc0_path0", usb_file_part);
							//nvram_set("usb_disc0_fs_path0", usb_part);
							nvram_set("usb_disc0_fs_path0", dev_real_path);	// 2007.11 James.
							nvram_set("usb_disc0_index", "1");
						}
						//else if (strncmp(usb_part, nvram_safe_get("usb_disc1_dev"), 16)==0)
						else if(!strncmp(usb_part, nvram_safe_get("usb_disc1_dev"), 33))	// 2007.11 James.
						{
							nvram_set("usb_disc1_path0", usb_file_part);
							//nvram_set("usb_disc1_fs_path0", usb_part);
							nvram_set("usb_disc1_fs_path0", dev_real_path);	// 2007.11 James.
							nvram_set("usb_disc1_index", "1");
						}
						n++;
						p++;
					}
					else
					{
						rmdir(usb_file_part);
						//printf("mount %s failed\n", usb_part);
						printf("mount %s failed\n", dev_real_path);	// 2007.11 James.
					}
#endif
				}
				
				printf("\n\n**********************************************\n");
				write_disc_status();
				printf("**********************************************\n\n");
			}
		}
	}
#endif //LINUX26
#ifdef DLM
eject:

	if(p
			|| (nvram_match("eject_from_web", "1")
				&& (nvram_invmatch("usb_disc0_dev", "") || nvram_invmatch("usb_disc1_dev", "")))
			)// if any usb mass storage device newly mounted at this time
	{	
		nvram_set("eject_from_web", "0");


		start_script(new_disc); // Move ahead to speed up WCN

		if(!nvram_match("reboot_WCN", "2")) {
			run_ftpsamba();
		}

		run_apps();
	}
#else
	if (p)	// if any usb mass storage device newly mounted at this time
	{
		start_script(new_disc);
	}
#endif

	if(usb_dev_disc)
		closedir(usb_dev_disc);
	if(usb_dev_part)
		closedir(usb_dev_part);
	if(dir_to_open)
		closedir(dir_to_open);

	return 0;
}
#else
int
hotplug_usb_mass(char *product)	// modified by Jiahao for WL500gP
{
	return 0;
}

#endif

/* plugging or removing usb device */
/* usbcore, usb-ohci, usb-ehci, printer are always there */
/* usb-storage, sd_mod, scsi_mod, videodev are there if functions are enabled */
/* pwc, ov511 i2c, depends on current status */

int
hotplug_usb(void)
{
	char *action=NULL, *interface=NULL, *product=NULL, *usb_path;
	const char *temp, *type;
	int i;
	int isweb;
	char flag[6];
	char tmp_str[16];
#ifdef U2EC
	int u2ec_fifo;
#endif
	char usbpath_nvram[16];
	
	product = getenv("PRODUCT");
	if(product == NULL || !strcmp(product, "0/0/206"))
		return EINVAL;
	else
		nvram_set("usb_vidpid", product);
	
	usb_path = getenv("USBDEVICE_PATH");
	
	if(strcmp(usb_path, "1") && strcmp(usb_path, "2"))
		usb_path = NULL;
	
	if(usb_path){
		nvram_set("usb_info", "set_usb_path");
		nvram_set("usb_path", usb_path);
		
		memset(usbpath_nvram, 0, 16);
		sprintf(usbpath_nvram, "usb_path%s", usb_path);
	}
#if 0
	action = getenv("ACTION");
	interface = getenv("INTERFACE");
	if (action)
		nvram_set("INTERFACE", interface);
	if (interface)
		nvram_set("ACTION", action);


#endif
	if( !(interface = getenv("INTERFACE")) || 
	    !(action = getenv("ACTION"))) 
	{
		printf("INTERFACE or ACTION read fails!\n");
		return EINVAL;
	}
#if 0
	if (!(action = getenv("ACTION"))) {
		nvram_set ("usb_info2", "err");
	}
	else nvram_set("ACTION", action);

	if (!(interface = getenv("INTERFACE"))) {
		nvram_set ("usb_info3", "err");
	}
	else nvram_set("INTERFACE", interface);

	if (!(product = getenv("PRODUCT"))) {
		nvram_set ("usb_info4", "err");
	}
	else nvram_set("PRODUCT", product);

	if (!(type = getenv("TYPE"))) {
		nvram_set ("usb_info5", "err");
	}
	else nvram_set("type", type);
#endif
#if 0
	if (!(temp = getenv("INFA"))) {
		nvram_set ("usb_info6", "err");
	}
	else nvram_set("INFA", temp);
#endif
	if (product)
	{
		nvram_set("usb_info2","product");
		if(strncmp(interface, "8/", 2)==0) //2008.03.13 Yau add usb_mass_storage
		{
			//logmessage("USB mass storage", "attached");
			
			isweb = MASS_STORAGE;
			goto usbhandler;
		}
#ifdef DLM
		else if (strncmp(interface, "1/1", 3)==0)
		{
			// if the audio device is the same with web cam,
			// just skip it
			if (nvram_match("usb_web_device", product)) 
				return 0;
			
			isweb = WEB_AUDIO;
			goto usbhandler;
		}
#endif
		else if(strncmp(interface, "7/", 2)==0) //2008.03.13 Yau add printer log
		{
// 2008.05 James. {
			//logmessage("USB printer", "attached");
			isweb = USB_PRINTER;
			goto usbhandler;
// 2008.05 James. }
		}
		else if(strncmp(interface, "1/", 2)==0)
		{
			return 0;
		}
		i=0;
		isweb = WEB_NONE;
#ifdef DLM
		while(PWCLIST[i]!=NULL)
		{
			if (strstr(product, PWCLIST[i]))
			{
				isweb = WEB_PWCWEB;
				goto usbhandler;
			}
			i++;
		}

		i=0;
		while(OVLIST[i]!=NULL)
		{
			if (strstr(product, OVLIST[i]))
			{
				isweb = WEB_OVWEB;
				goto usbhandler; 
			}
			i++;
		}
#endif
	}
	else
		return EINVAL;
usbhandler:
csprintf("- hotplug(%d): action=%s, interface=%s, product=%s, usb_path=%s. -\n"
		, isweb
		, action
		, interface
		, product
		, usb_path
		);
	nvram_set("usb_info3", "usbhandler");
	if ((strcmp(action, "add")) == 0)
	{
		/*if(nvram_match("usb_device", "1"))
			nvram_set("usb_device_2", "1");
		else
			nvram_set("usb_device", "1");//*/
		
		if (isweb==MASS_STORAGE) //2008.03.13 Yau modify
		{
/*			nvram_set("asus_mfg_flash", "1");
			
// 2008.06 James. {
			if(strlen(nvram_safe_get("usb_disc0_pid")) <= 0){
				nvram_set("usb_disc0_pid", product);
				nvram_set("usb_disc0_port", nvram_get("usb_path"));
			}
			else if(strlen(nvram_safe_get("usb_disc1_pid")) <= 0){
				nvram_set("usb_disc1_pid", product);
				nvram_set("usb_disc1_port", nvram_get("usb_path"));
			}
			else
				return 0;
// 2008.06 James. }
			
			nvram_set("usb_storage_device", product); //watchdog will call ftpd if set
			if(usb_path && strlen(nvram_safe_get(usbpath_nvram)) <= 0)
				nvram_set(usbpath_nvram, "storage");
			logmessage("USB storage", "\"%s\" was plugged in.", product);	// 2008.02 James. */
                        if(usb_path && strlen(nvram_safe_get(usbpath_nvram)) <= 0){
                                nvram_set("asus_mfg_flash", "1");

// 2008.06 James. {
                                if(strlen(nvram_safe_get("usb_disc0_pid")) <= 0){
                                        nvram_set("usb_disc0_pid", product);
                                        nvram_set("usb_disc0_port", usb_path);
                                }
                                else if(strlen(nvram_safe_get("usb_disc1_pid")) <= 0){
                                        nvram_set("usb_disc1_pid", product);
                                        nvram_set("usb_disc1_port", usb_path);
                                }
                                else
                                        return 0;

// 2008.06 James. }

				nvram_set("usb_storage_device", product); //watchdog will call ftpd if set
				nvram_set(usbpath_nvram, "storage");
                                logmessage("USB storage", "\"%s\" was plugged in.", product);   // 2008.02 James.
			}
		}
#ifdef DLM
		else if (isweb==WEB_AUDIO)
		{
			nvram_set("asus_mfg_audio", "1");
			
			if (nvram_match("usb_audio_device", ""))
				logmessage("USB audio", "attached");
			nvram_set("usb_audio_device", product);
			if(usb_path){
				nvram_set(usbpath_nvram, "audio");
				nvram_set("usb_path_audio", usb_path);
			}
			refresh_wave();
		}
#endif
// 2008.05 James. {
		else if (isweb==USB_PRINTER)
		{
			nvram_set("asus_mfg_printer", "1");
			
			if (nvram_match("usb_printer_device", ""))
				logmessage("USB printer", "\"%s\" was plugged in.", product);
			nvram_set("usb_printer_device", product);
			if(usb_path){
				nvram_set(usbpath_nvram, "printer");
				nvram_set("usb_path_printer", usb_path);
			}
		}
// 2008.05 James. }
#ifdef DLM
		else if (isweb==WEB_PWCWEB || isweb==WEB_OVWEB)
		{
			/* Add MFG test */
			nvram_set("asus_mfg_webcam", "1");
			
			if (nvram_match("usb_web_device", ""))
				logmessage("USB webcam", "attached");

			sprintf(flag, "%d", isweb);
			nvram_set("usb_web_device", product);
			nvram_set("usb_web_flag", flag);
			nvram_set("usb_webdriver_x", "");
			if(usb_path){
				nvram_set(usbpath_nvram, "webcam");
				nvram_set("usb_path_webcam", usb_path);
			}
		}
#endif
/*
		else{
			nvram_set("asus_mfg_flash", "1");
// 2008.06 James. {
			if(strlen(nvram_safe_get("usb_disc0_pid")) <= 0){
				nvram_set("usb_disc0_pid", product);
				nvram_set("usb_disc0_port", nvram_get("usb_path"));
			}
			else if(strlen(nvram_safe_get("usb_disc1_pid")) <= 0){
				nvram_set("usb_disc1_pid", product);
				nvram_set("usb_disc1_port", nvram_get("usb_path"));
			}
			else
				return 0;
// 2008.06 James. }
			
			nvram_set("usb_storage_device", product); //watchdog will call ftpd if set
			if(usb_path && strlen(nvram_safe_get(usbpath_nvram)) <= 0)
				nvram_set(usbpath_nvram, "storage");
			logmessage("USB storage", "\"%s\" was plugged in.", product);   // 2008.02 James.
		}
*/		
#ifdef U2EC
		u2ec_fifo = open("/var/u2ec_fifo", O_WRONLY|O_NONBLOCK|O_CREAT);
		write(u2ec_fifo, "a", 1);
		close(u2ec_fifo);
#endif
	}
	else //USB device removed
	{
/*		if(usb_path)
			nvram_set(usbpath_nvram, "");
*/		
		//if (isweb==WEB_NONE) // Treat it as USB Storage
		if (isweb==MASS_STORAGE)	// 2008.02 James.
		{
			logmessage("USB device", "\"%s\" was removed.", product);	// 2008.02 James.
			// Yen
			if (nvram_match("apps_dl_x", "1")) { // Reset some DM values
				nvram_set ("apps_dl_x","0");
				nvram_set ("apps_status_checked", "1");
				if (nvram_match("apps_dms_usb_port_x","0")) {
					nvram_set ("usb_disc0_dev", "");
					nvram_set ("usb_disc0_port", "0");
				}
				else {
					nvram_set ("usb_disc1_dev", "");
					nvram_set ("usb_disc1_port", "0");
				}
			}

// 2009.0623 James. {
			/*if(!strcmp(nvram_safe_get("usb_disc0_pid"), product)){
				nvram_set("usb_disc0_pid", "");
				nvram_set("usb_disc0_port", "0");
				nvram_set("usb_disc0_safely_removed", "0");
			}
			else if(!strcmp(nvram_safe_get("usb_disc1_pid"), product)){
				nvram_set("usb_disc1_pid", "");
				nvram_set("usb_disc1_port", "0");
				nvram_set("usb_disc1_safely_removed", "0");
			}
			else
				return 0;//*/
// 2009.0623 James. }
			
// 2007.12 James {
			//remove_usb_mass(product);
			nvram_set("usb_storage_device_remove", product);
// 2007.12 James }
		}
#ifdef DLM
		else if(isweb==WEB_AUDIO)
		{
			logmessage("USB audio", "removed");
			remove_usb_audio(product);
			nvram_set("usb_audio_device", "");
			nvram_set("usb_path_audio", "");
		}
#endif
		else if(isweb==USB_PRINTER)
		{
			logmessage("USB printer", "removed");
			nvram_set("usb_printer_device", "");
			nvram_set("usb_path_printer", "");
		}
#ifdef DLM
		else if(isweb==WEB_PWCWEB || isweb==WEB_OVWEB)
		{
			if (nvram_invmatch("usb_web_device", ""))
			{
				logmessage("USB webcam", "removed");
				nvram_set("usb_web_device", "");
				nvram_set("usb_web_flag", "");
				nvram_set("usb_path_webcam", "");
			}
			remove_usb_webcam(product, isweb);
		}
#endif
		else{
			logmessage("USB device", "\"%s\" was removed.", product);       // 2008.02 James.
			
// 2008.06 James. {
			if(!strcmp(nvram_safe_get("usb_disc0_pid"), product)){
				nvram_set("usb_disc0_pid", "");
				nvram_set("usb_disc0_port", "0");
				nvram_set("usb_disc0_safely_removed", "0");
			}
			else if(!strcmp(nvram_safe_get("usb_disc1_pid"), product)){
				nvram_set("usb_disc1_pid", "");
				nvram_set("usb_disc1_port", "0");
				nvram_set("usb_disc1_safely_removed", "0");
			}
			else
				return 0;
// 2008.06 James. }
// 2007.12 James {
			//remove_usb_mass(product);
			nvram_set("usb_storage_device_remove", product);
// 2007.12 James }
		}
#ifdef U2EC
		u2ec_fifo = open("/var/u2ec_fifo", O_WRONLY|O_NONBLOCK);
		write(u2ec_fifo, "r", 1);
		close(u2ec_fifo);
#endif
		if(usb_path)
                        nvram_set(usbpath_nvram, "");
	}

	return 0;
}
#endif

/* stop necessary services for firmware upgrade */	
/* stopservice: for firmware upgarde */
/* stopservice 1: for button setup   */
int
stop_service_main(int type)
{
	if (type==1)
	{
		//stop_misc();
		//stop_logger();
		stop_usb();
		stop_nas();
		stop_upnp();
		//stop_dhcpd();
		stop_dns();
		stop_httpd();
		eval("killall", "udhcpc");
		//eval("killall", "infosvr");
	}
	else
	{
		if(type==99) stop_misc_no_watchdog();
		else stop_misc();
		stop_logger();
		stop_usb();

		//stop_nas();	// mark this for upgrade firmware with wireless.
		stop_upnp();
		stop_dhcpd();
		stop_dns();
	}

	dprintf("done\n");
	return 0;
}

int service_handle(void)
{
	char *service;
	char tmp[100], *str;
	int pid;
	char *ping_argv[] = { "ping", "140.113.1.1", NULL};
	FILE *fp;

	service = nvram_get("rc_service");

	if(!service)
		kill(1, SIGHUP);

	if(strstr(service,"wan_disconnect")!=NULL)
	{
		cprintf("wan disconnect\n");

		logmessage("WAN Connection", "Disconnected manually");

		if (nvram_match("wan0_proto", "dhcp")
#ifdef CDMA
				|| nvram_match("wan0_proto", "cdma")
#endif
			)
		{		
			snprintf(tmp, sizeof(tmp), "/var/run/udhcpc%d.pid", 0);
			if ((str = file2str(tmp))) {
				pid = atoi(str);
				free(str);			
				kill(pid, SIGUSR2);
			}
		}
		else 
		{			
			stop_wan2();
			update_wan_status(0);
			sleep(3);
		}
	}
	else if (strstr(service,"wan_connect")!=NULL)
	{
		cprintf("wan connect\n");
		logmessage("WAN Connection", "Connected manually");
		setup_ethernet(nvram_safe_get("wan_ifname"));

		if (nvram_match("wan0_proto", "dhcp")
#ifdef CDMA
				|| nvram_match("wan0_proto", "cdma")
#endif
			)
		{
			snprintf(tmp, sizeof(tmp), "/var/run/udhcpc%d.pid", 0);
			if ((str = file2str(tmp))) {
				pid = atoi(str);
				free(str);
				kill(pid, SIGUSR1);
			}
		}
		else 
		{
#ifndef REMOVE
			// pppoe or ppptp, check if /tmp/ppp exist
			if (nvram_invmatch("wan0_proto", "static") && (fp=fopen("/tmp/ppp/ip-up", "r"))!=NULL)
			{
				fclose(fp);
				_eval(ping_argv, NULL, 0, &pid);
			}
			else
			{
				stop_wan();
				sleep(3);
				start_wan();
				sleep(2);
				_eval(ping_argv, NULL, 0, &pid);
			}
#endif

#ifdef REMOVE
			stop_wan();
			sleep(2);
			start_wan();
			/* trigger connect */
			eval("ntpclient", "-h", "test", "-c", "1");
#endif

		}
	}
#ifdef ASUS_DDNS //2007.03.26 Yau add for asus ddns
	else if(strstr(service,"ddns_hostname_check") != NULL)
	{
		char host[64];
		char wan_ifname[16];
		
		strcpy(host, nvram_get("ddns_hostname_x"));
		if(nvram_match("wan_proto", "pppoe") || nvram_match("wan_proto", "pptp") || nvram_match("wan_proto", "l2tp"))
			strcpy(wan_ifname, nvram_get("wan0_pppoe_ifname"));
		else
			strcpy(wan_ifname, nvram_get("wan0_ifname"));
		
		char *ddns_argv[] = {"ez-ipupdate",
												 "-h", host,
												 "-s", "ns1.asuscomm.com",
												 "-S", "dyndns",
												 "-i", wan_ifname,
												 "-A", "1",
												 NULL};
		pid_t pid;
		
		//Execute ez-ipupdate then die.
		eval("killall", "ez-ipupdate");
		_eval(ddns_argv, NULL, 0, &pid);
	}
#endif

	nvram_unset("rc_service");
	return 0;
}

#ifdef AUDIO_SUPPORT
int hotplug_usb_audio(char *product)
{					
	char *wave_argv[]={"waveserver", NULL};
	pid_t pid;

	if (strlen(product)==0) return;
	return _eval(wave_argv, ">/dev/null", 0, NULL);	// 2007.11 James. add "return"
}

int remove_usb_audio(char *product)
{
	return eval("killall", "waveserver");	// 2007.11 James. add "return"
}

int
start_audio(void)
{
	char *wave_argv[] = {"waveservermain", NULL};
	pid_t pid;

	_eval(wave_argv, NULL, 0, &pid);
	return 0;
}

int
stop_audio(void)
{
	int ret = eval("killall", "waveserver");
	return ret;	// 2007.11 James.
}
#endif
#endif

#ifdef GUEST_ACCOUNT
int
start_dhcpd_guest(void)
{
	FILE *fp;
	char *dhcpd_argv[] = {"udhcpd", "/tmp/udhcpd1.conf", NULL, NULL};
	char *slease = "/tmp/udhcpd-br1.sleases";
	pid_t pid;

	if (nvram_match("router_disable", "1") || nvram_invmatch("wl_guest_enable", "1") || nvram_invmatch("lan1_proto", "dhcp"))
		return 0;

	dprintf("%s %s %s %s\n",
		nvram_safe_get("lan1_ifname"),
		nvram_safe_get("dhcp1_start"),
		nvram_safe_get("dhcp1_end"),
		nvram_safe_get("lan1_lease"));

	if (!(fp = fopen("/tmp/udhcpd-br1.leases", "a"))) {
		perror("/tmp/udhcpd-br1.leases");
		return errno;
	}
	fclose(fp);

	/* Write configuration file based on current information */
	if (!(fp = fopen("/tmp/udhcpd1.conf", "w"))) {
		perror("/tmp/udhcpd1.conf");
		return errno;
	}
	
	fprintf(fp, "pidfile /var/run/udhcpd-br1.pid\n");
	fprintf(fp, "start %s\n", nvram_safe_get("dhcp1_start"));
	fprintf(fp, "end %s\n", nvram_safe_get("dhcp1_end"));
	fprintf(fp, "interface %s\n", nvram_safe_get("lan1_ifname"));
	fprintf(fp, "remaining yes\n");
	fprintf(fp, "lease_file /tmp/udhcpd-br1.leases\n");
	fprintf(fp, "option subnet %s\n", nvram_safe_get("lan1_netmask"));
	fprintf(fp, "option router %s\n", nvram_safe_get("lan1_ipaddr"));	
	
	if (nvram_invmatch("dhcp_dns1_x",""))		
		fprintf(fp, "option dns %s\n", nvram_safe_get("dhcp_dns1_x"));		
	fprintf(fp, "option dns %s\n", nvram_safe_get("lan1_ipaddr"));
	fprintf(fp, "option lease %s\n", nvram_safe_get("lan1_lease"));

	if (nvram_invmatch("dhcp_wins_x",""))		
		fprintf(fp, "option wins %s\n", nvram_safe_get("dhcp_wins_x"));		
	if (nvram_invmatch("lan_domain", ""))
		fprintf(fp, "option domain %s\n", nvram_safe_get("lan_domain"));
	fclose(fp);

	dhcpd_argv[2] = NULL;
	_eval(dhcpd_argv, NULL, 0, &pid);


	dprintf("done\n");
	return 0;
}

int
stop_dhcpd_guest(void)
{
	char sigusr1[] = "-XX";
	int ret;

/*
* Process udhcpd handles two signals - SIGTERM and SIGUSR1
*
*  - SIGUSR1 saves all leases in /tmp/udhcpd.leases
*  - SIGTERM causes the process to be killed
*
* The SIGUSR1+SIGTERM behavior is what we like so that all current client
* leases will be honorred when the dhcpd restarts and all clients can extend
* their leases and continue their current IP addresses. Otherwise clients
* would get NAK'd when they try to extend/rebind their leases and they 
* would have to release current IP and to request a new one which causes 
* a no-IP gap in between.
*/
	ret = eval("killall", "udhcpd");

	dprintf("done\n");
	return ret;
}
#endif

#ifdef DLM
// 2007.11 James. replace this with using eval("/sbin/write_smb_conf") for new samba mode.
void write_smb_conf()
{
	struct dirent *dp;
	FILE *fp;
	DIR *dir_to_open=NULL;
	int n=0, sh_num=0;
	char *tmp1=NULL;
	char SHMAX[32];
	char SMB_SHNAME[64];
	char SHNM[16];
	char SMB_SHCOMMENT[64];
	char SHCM[16];
	char SMB_SHPATH[104];
	char SHPH[32];
	char SHAU[16];
	char SMB_SHRRIGHT[384];
	char SHRR[384];
	char SMB_SHWRIGHT[384];
	char SHWR[384];
	char SMB_SHACCUSER[384];
	
// write smb.conf
	if((fp=fopen("/tmp/samba/lib/smb.conf", "r")))
	{
		fclose(fp);
		//eval("rm", "-f", "/tmp/samba/lib/smb.conf");
		unlink("/tmp/samba/lib/smb.conf");	// 2008.08 James.
	}
	fp = fopen("/tmp/samba/lib/smb.conf", "w");
	fprintf(fp, "[global]\n");
	if(nvram_safe_get("st_samba_workgroup"))
		fprintf(fp, "workgroup = %s\n", nvram_safe_get("st_samba_workgroup"));
	if(nvram_safe_get("computer_name"))
		fprintf(fp, "netbios name = %s\n", nvram_safe_get("computer_name"));
	fprintf(fp, "server string = WL500g.Premium version 2\n");
	eval("mkdir", "-p", "/tmp/var/log/samba");
	fprintf(fp, "log file = /tmp/var/log/samba/samba.log\n");
	fprintf(fp, "log level = 0\n");
	fprintf(fp, "max log size = 5\n");
 
// share mode
	if(!strcmp(nvram_safe_get("st_samba_modex"), "1"))
	{
		fprintf(fp, "security = SHARE\n");
		fprintf(fp, "guest ok = yes\n");
		fprintf(fp, "guest only = yes\n");
	}
	else
	{
		fprintf(fp, "security = USER\n");
		fprintf(fp, "guest ok = no\n");
		fprintf(fp, "map to guest = Bad User\n");
	}
	fprintf(fp, "writeable = yes\n");
	fprintf(fp, "directory mode = 0777\n");
	fprintf(fp, "create mask = 0777\n");
// max users
	sprintf(SHMAX, "st_max_user");
	if(strcmp(nvram_safe_get(SHMAX), "")!=0)
		fprintf(fp, "max connections = %s\n", nvram_safe_get(SHMAX));
	fprintf(fp, "encrypt passwords = yes\n");
	fprintf(fp, "pam password change = no\n");
	fprintf(fp, "obey pam restrictions = no\n");
	fprintf(fp, "socket options = TCP_NODELAY SO_KEEPALIVE SO_RCVBUF=16384 SO_SNDBUF=16384\n");
	fprintf(fp, "dns proxy = no\n");
// share
	if(!strcmp(nvram_safe_get("st_samba_modex"), "0") || !strcmp(nvram_safe_get("st_samba_modex"), ""))
	{
		;
	}
	else if(!strcmp(nvram_safe_get("st_samba_modex"), "1"))
	{
		printf("samba mode: share\n");
		if(!(dir_to_open = opendir("/tmp/harddisk")))
		{
			perror("***cannot open /tmp/harddisk\n");
			goto confpage;
		}
		n = 0;
		if(dir_to_open)
		{
			while(dp=readdir(dir_to_open))
			{
				if(strncmp(dp->d_name, "..", NAME_MAX) != 0 && strncmp(dp->d_name, ".", NAME_MAX) != 0 )
				{
					sprintf(SMB_SHNAME, "%s", dp->d_name);
					sprintf(SMB_SHCOMMENT, "%s", dp->d_name);
					sprintf(SMB_SHPATH, "/tmp/harddisk/%s", dp->d_name);
					printf("%s\n", SMB_SHPATH);
					fprintf(fp, "[%s]\n", SMB_SHNAME);
					fprintf(fp, "comment = %s\n", SMB_SHCOMMENT);
					fprintf(fp, "path = %s\n", SMB_SHPATH);
					fprintf(fp, "guest ok = yes\n");
					fprintf(fp, "writeable = yes\n");
					fprintf(fp, "directory mode = 0777\n");
					fprintf(fp, "create mask = 0777\n");
				}
			}
		}
		if(dir_to_open)
			closedir(dir_to_open);
	}
	else // st_samba_mode = 2
	{
	printf("samba mode: user\n");
	n = 0;
	sh_num = atoi(nvram_safe_get("sh_num"));
	while(n<sh_num)
	{
		sprintf(SHPH, "sh_path%d", n);
		sprintf(SHNM, "sh_name%d", n);
		sprintf(SHRR, "sh_rright%d", n);
		sprintf(SHWR, "sh_wright%d", n);
		sprintf(SHCM, "sh_comment%d", n);
		sprintf(SHAU, "sh_acc_user%d", n);
		sprintf(SMB_SHPATH, "/tmp/harddisk%s", nvram_safe_get(SHPH));
		sprintf(SMB_SHNAME, "%s", nvram_safe_get(SHNM));
		sprintf(SMB_SHRRIGHT, "%s", nvram_safe_get(SHRR));
		sprintf(SMB_SHWRIGHT, "%s", nvram_safe_get(SHWR));
		sprintf(SMB_SHACCUSER, "%s", nvram_safe_get(SHAU));

		while(tmp1=strchr(SMB_SHRRIGHT, ';'))
			memcpy(tmp1, " ", 1);
		memcpy(SMB_SHRRIGHT+strlen(SMB_SHRRIGHT)-1, "\0", 1);
		while(tmp1=strchr(SMB_SHWRIGHT, ';'))
			memcpy(tmp1, " ", 1);
		memcpy(SMB_SHWRIGHT+strlen(SMB_SHWRIGHT)-1, "\0", 1);
		while(tmp1=strchr(SMB_SHACCUSER, ';'))
			memcpy(tmp1, " ", 1);
		memcpy(SMB_SHACCUSER+strlen(SMB_SHACCUSER)-1, "\0", 1);
		sprintf(SMB_SHCOMMENT, "%s", nvram_safe_get(SHCM));
		// write to conf
		if(!strcmp(SMB_SHNAME, ""))
		{
			goto endloop;
		}
		if(!(dir_to_open = opendir(SMB_SHPATH)))
		{
			goto endloop;
		}
		else
			closedir(dir_to_open);
		fprintf(fp, "[%s]\n", SMB_SHNAME);
		fprintf(fp, "comment = %s\n", SMB_SHCOMMENT);
		fprintf(fp, "path = %s\n", SMB_SHPATH);
		if(strstr(SMB_SHWRIGHT, "Guest"))
		{
				fprintf(fp, "guest ok = yes\n");
		}
		else
		{
			if(strstr(SMB_SHRRIGHT, "Guest"))
			{
				fprintf(fp, "guest ok = yes\n");
				fprintf(fp, "writeable = no\n");
				fprintf(fp, "write list = %s\n", SMB_SHWRIGHT);
			}
			else
			{
				if(!strcmp(SMB_SHWRIGHT, "")&&!strcmp(SMB_SHRRIGHT, ""))
					fprintf(fp, "valid users = _an_si_un_se_shorti_\n");
				else
					fprintf(fp, "valid users = %s\n", SMB_SHACCUSER);
				fprintf(fp, "writeable = no\n");
				fprintf(fp, "write list = %s\n", SMB_SHWRIGHT);
				fprintf(fp, "read list = %s\n", SMB_SHRRIGHT);
			}
		}
		fprintf(fp, "directory mode = 0777\n");
		fprintf(fp, "create mask = 0777\n");
		// write to conf
endloop:
		n++;
	}
	}// st_samba_mode = 2
confpage:	
	fclose(fp);
}

int
start_samba(void)	// added by Jiahao for WL500gP
{
//	FILE *fp;
	FILE *f;
	int n=0, sh_num=0, i;
//	char *tmp1=NULL, *tmp2=NULL;
	char tmp1[33], tmp2[33];
	int ret1=0, ret2=0;
	char *name, *pass;

	stop_samba(); // 2007.11 James. for new samba mode
	
	if (nvram_match("samba_enable","0"))
		return 1;
/* write samba configure file*/
	//write_smb_conf();
	eval("/sbin/write_smb_conf");	// 2007.11 James. for new samba mode

/* write /etc/passwd */
	/* crypt using md5, no salt */
        name = nvram_get("http_username") ? : "admin";
        pass = crypt(nvram_get("http_passwd") ? : "admin", "$1$");

        if ((f = fopen("/tmp/passwd", "w"))) 
	{
		fprintf(f, "%s:%s:0:0:Administrator:/:/bin/sh\n"
                        "nobody:x:99:99:nobody:/:/sbin/nologin\n", name, pass);
 
                sh_num = atoi(nvram_safe_get("acc_num"));
                for (i=0, n=500; i<sh_num; i++, n++)
                {
                        sprintf(tmp1, "acc_username%d", i);
                        fprintf(f, "%s:x:%d:%d:::\n", nvram_safe_get(tmp1), n, n);
                }
                fclose(f);
	}
/* write /etc/group  */
	if ((f = fopen("/tmp/group", "w"))) {
                fprintf(f, "root:x:0:%s\nnobody:x:99:\n", name);
               
                sh_num = atoi(nvram_safe_get("acc_num"));
                for (i=0, n=500; i<sh_num; i++, n++)
                {
                        sprintf(tmp1, "acc_username%d", i);
                        fprintf(f, "%s:x:%d:\n", nvram_safe_get(tmp1), n);
                }

                fclose(f);
	}
/* write smbpasswd  */
	eval("/usr/local/samba/bin/smbpasswd", "-a", "nobody", "\"\"");
	sh_num = atoi(nvram_safe_get("acc_num"));
	for(i=0; i<sh_num; i++)
	{
		sprintf(tmp1, "acc_username%d", i);
		sprintf(tmp2, "acc_password%d", i);
		sprintf(tmp1, nvram_safe_get(tmp1));
		sprintf(tmp2, nvram_safe_get(tmp2));
		eval("/usr/local/samba/bin/smbpasswd", "-a", tmp1, tmp2);
	}

	ret1 = eval("/usr/local/samba/sbin/smbd", "-D", "-s", "/tmp/samba/lib/smb.conf");
	ret2 = eval("/usr/local/samba/sbin/nmbd", "-D", "-s", "/tmp/samba/lib/smb.conf");

	if(!ret1 && !ret2)
	{
		//if (nvram_match("st_samba_modex", "1"))
		if(nvram_match("st_samba_mode", "1") || nvram_match("st_samba_mode", "3"))	// 2007.11 James.
			nvram_set("st_samba_mode_x", "1");
		else
			nvram_set("st_samba_mode_x", "0");

		fprintf(stderr, "Starting samba\n");
		logmessage("Samba Server", "daemon is started");
		return 0;
		
	}
	else
	{
		eval("killall", "-SIGKILL", "smbd");
		eval("killall", "-SIGKILL", "nmbd");
		nvram_set("st_samba_mode_x", "0");	// 2007.11 James.
		return 1;
	}
}
#endif

int
check_disk_free(char *res, char *diskpath)	// added by Jiahao for WL500gP
{
	char *sizebuf, *freebuf, *databuf;
	struct statfs fsbuf;

	if(statfs(diskpath, &fsbuf))
	{
		perror("*** check_disk_free: statfs fail!");
		return 2;
	}

	sizebuf = malloc(32);
	freebuf = malloc(32);
	databuf = malloc(64);

	sprintf(sizebuf, "%.1f", (double)((double)((double)fsbuf.f_blocks * fsbuf.f_bsize)/(1024*1024)));
	sprintf(freebuf, "%.1f", (double)((double)((double)fsbuf.f_bfree * fsbuf.f_bsize)/(1024*1024)));
	sprintf(res, "size: %s, free: %s", sizebuf, freebuf);

	if(sizebuf) 
		free(sizebuf);
	if(freebuf) 
		free(freebuf);
	if(databuf) 
		free(databuf);
	if(fsbuf.f_bfree == 0)
		return 0;
	else
		return 1;
}

#ifdef DLM
int
check_disk_free_GE_1G(char *diskpath)	// added by Jiahao for WL500gP
{
	struct statfs fsbuf;

	if(statfs(diskpath, &fsbuf))
	{
		perror("*** check_disk_free_GE_1G: statfs fail!");
		return 0;
	}

	if ( (double)((double)((double)fsbuf.f_bfree * fsbuf.f_bsize)/(1024*1024)) < (double)33 )
		return 1;
	else if ( (double)((double)((double)fsbuf.f_blocks * fsbuf.f_bsize)/(1024*1024)) > (double)2048 )
		return 2;
	else
		return 3;
}

int
check_disk_free_apps(char *diskpath, int ac_flag)			// added by Jiahao for WL500gP
{
	struct statfs fsbuf;

	if(statfs(diskpath, &fsbuf))
	{
		perror("*** check_disk_free_apps: statfs fail!");
		return 0;
	}

	if (ac_flag==1)
	{
		if (nvram_match("apps_dlx", "0"))
			return 1;

		if ( (double)((double)((double)fsbuf.f_bfree * fsbuf.f_bsize)/(1024*1024)) < (double)1 )
			return 0;
		else
			return 1;
	}
	else
	{
		if ( (double)((double)((double)fsbuf.f_bfree * fsbuf.f_bsize)/(1024*1024)) < (double)5 )
			return 0;
		else
			return 1;
	}
}
#endif

void
get_disc_desc(char * vendor_product, int discorder, int discindex) {	// added by Jiahao for WL500gP
	FILE* fp;
	DIR *dir_to_open, *dir_to_open2;
	struct dirent *dp, *dp2;
	char str_path[128];
	char temp[80];
	char temp2[80];
	char temp3[32];
	char temp4[64];
	char temp5[64];
	char port[128];
	char tempstr[1];
	char host[128];
	char *p, *p2;
	
	strcpy(vendor_product, "");
	
	if( dir_to_open = opendir("/proc/scsi") )
	{
		while(dp=readdir(dir_to_open))
		{
			if(	!strncmp(dp->d_name, "..", NAME_MAX)	|| 
		           	!strncmp(dp->d_name, ".", NAME_MAX)	||
		           	!strncmp(dp->d_name, "sg", NAME_MAX)	||
		          	!strncmp(dp->d_name, "scsi", NAME_MAX)	)
				continue;
			sprintf(str_path, "/proc/scsi/%s", dp->d_name);
			if (dir_to_open2 = opendir(str_path))
			{
				while(dp2=readdir(dir_to_open2))
				{
					if(	!strncmp(dp2->d_name, "..", NAME_MAX)	|| 
		           			!strncmp(dp2->d_name, ".", NAME_MAX)	)
						continue;
#if 0
					sprintf(temp3, "/dev/discs/disc%d", discorder);
					sprintf(temp5, "../scsi/host%s/bus0/target0/lun0", dp2->d_name);
					readlink(temp3, temp4, 64);
					if (strncmp(temp5, temp4, 13)!=0)
						break;
					else
					{

#endif
						if (fp=fopen("/proc/scsi/scsi", "r"))
						{
							while (fgets(temp,80,fp)!=NULL)
							{
								if (p=strstr(temp, "Host: scsi"))
								{
									p+=10;
									sprintf(host, p);
									memcpy(host+1, "\0", 1);
									continue;
								}
								if (p=strstr(temp, "Vendor: "))
								{
									if(!strcmp(dp2->d_name, host))
									{
										sprintf(vendor_product, p);
										p2=strstr(temp, "Rev: ");
										memcpy(vendor_product+strlen(p)-strlen(p2)-1, "\0", 1);
										break;
									}
								}
							}
							fclose(fp);
						}

						sprintf(str_path, "/proc/scsi/%s/%s", dp->d_name, dp2->d_name);
						if (fp=fopen(str_path, "r"))
						{
							while (fgets(temp,80,fp)!=NULL)
							{
								if (strstr(temp, "Port: "))
								{
									if (p=strstr(temp, ": "))
									{
										p+=2;
										sprintf(port, p);
										memcpy(port+strlen(port)-1, "\0", 1);
										
										sprintf(temp2, "usb_disc%d_port", discindex);
										//nvram_set(temp2, port);
										strcpy(tempstr, port);
										if (strcmp(tempstr, "1")==0)
											sprintf(port, "[USB Lower port]\n");
										else
											sprintf(port, "[USB Upper port]\n");
									}
									break;
								}
							}
							fclose(fp);
							strcat(port, vendor_product);
							strcpy(vendor_product, port);
						}
						else
							strcpy(vendor_product, "");
//					}
				}
				closedir(dir_to_open2);
			}
		}
		closedir(dir_to_open);
	}
}

void
write_per_disc_status(int disc_index)
{
	FILE *fp;
	char usb_disc_mount_path[128], usb_disc_fs_path[128];
	int i, j;
	char *temp=(char *)malloc(128);
	char disc_dev_str[32];
	char disc_idx_str[32];
	int disc_order[2];
	disc_order[0]=atoi(nvram_get("usb_disc0_dev")+14);
	disc_order[1]=atoi(nvram_get("usb_disc1_dev")+14);
	
	fp = fopen("/tmp/usstatus", "a");
	if (fp)
	{
		sprintf(disc_dev_str, "usb_disc%d_dev", disc_index);
		sprintf(disc_idx_str, "usb_disc%d_index", disc_index);
		if (nvram_invmatch(disc_dev_str, "") && nvram_invmatch(disc_idx_str, "0"))
		{
			get_disc_desc(temp, disc_order[disc_index], disc_index);
			printf("%s\n", temp);
			fprintf(fp, "%s\n", temp);
			i=atoi(nvram_safe_get(disc_idx_str));
			for(j=0; j<i; j++)
			{
				sprintf(usb_disc_mount_path, "usb_disc%d_path%d", disc_index, j);
				printf("%s ", nvram_safe_get(usb_disc_mount_path));
				sprintf(usb_disc_fs_path, "usb_disc%d_fs_path%d", disc_index, j);
				printf("%s ", nvram_safe_get(usb_disc_fs_path));					
				if (check_disk_free(temp, nvram_safe_get(usb_disc_mount_path))==2)
					sprintf(temp, "unknown");
				printf("%s\n", temp);
				fprintf(fp, "Partition %d: %s\n", j+1, temp);
			}
		}
		else
			fprintf(stderr, "skip write usstatus\n");
	}
	else
		fprintf(stderr, "can not open /tmp/usstatus\n");
	if (fp)
		fclose(fp);
}

void
write_disc_status()	// added by Jiahao for WL500gP
{
	int disc0_port=atoi(nvram_get("usb_disc0_port"));
	unlink("/tmp/usstatus");

	if (disc0_port==2)
	{
		write_per_disc_status(0);
		write_per_disc_status(1);
	}
	else
	{
		write_per_disc_status(1);
		write_per_disc_status(0);
	}
}

int                                     // added by Jiahao for WL500gP
check_if_file_exist(char *filename)
{
        FILE *fp;
        fp=fopen(filename, "r");
        if(fp)
        {
                fclose(fp);
                return 1;
        }
        else
                return 0;
}

int                                     // added by Jiahao for WL500gP
check_if_dir_exist(char *dir)
{
        DIR *dp;
        if(!(dp=opendir(dir)))
                return 0;
        closedir(dp);
        return 1;
}

int
mkdir_if_none(char *dir)                // added by Jiahao for WL500gP
{
        DIR *dp;
        if(!(dp=opendir(dir)))
        {
                umask(0000);
                mkdir(dir, 0777);
//              umask(0022);
                return 1;
        }
        closedir(dp);
        return 0;
}
#ifdef DLM
void set_apps_ver()
{
	nvram_set("apps_ver", "1.00");
}

void exec_dms()
{
	char name[32];
	char mymac[32];
	char path[256];
	char tmpstr2[256];
	int ret = 0;
	
	mkdir_if_none("/shares/DMSRoot");
	
	strcpy(name, nvram_safe_get("computer_name"));
	strcpy(mymac, nvram_safe_get("et0macaddr"));
	
	if(nvram_match("apps_dmsx", "1")){
		sprintf(path, "%s", nvram_get("first_partition"));
		sprintf(tmpstr2, "dms %s \"%s\" %s &", path, name, mymac);
		ret = system(tmpstr2);
		if(!ret)
			logmessage("Media Server", "daemon is started");
	}
}

void exec_apps()		// added by Jiahao for WL500gP
{	
	FILE *fp;
	
	char pool[32];
	char share[32];
	char rundl[8];
	char rundms[8];
	char name[32];
	char mymac[32];
	int cap=0;
	int ret=0;

	char EXBIN[32]="/.apps/bin";
	char test_path[128];

	char tmpstr[256];
	char tmpstr2[256];
	char share_port_from[5];
	char share_port_to[5];

	if ( nvram_match("apps_dms_usb_port_x", nvram_safe_get("usb_disc0_port")) )
	{
		strcpy(test_path, nvram_safe_get("usb_disc0_path0"));
	}
	else if ( nvram_match("apps_dms_usb_port_x", nvram_safe_get("usb_disc1_port")) )
	{
		strcpy(test_path, nvram_safe_get("usb_disc1_path0"));
	}
	strcpy(tmpstr, EXBIN);
	sprintf(EXBIN, "%s%s", test_path, tmpstr);
	
//	eval("killall", "dms");
	eval("killall", "ushare");
	eval("killall", "snarf");
	eval("killall", "ctorrent");
	eval("killall", "rtorrent");
	eval("killall", "giftd");
	eval("killall", "dmathined");
	
	nvram_set("dms_running", "0");	// 2007.11 James.
	//nvram_set("apps_installed", "0");	// 2007.11 James. set at the end of this function
	nvram_set("apps_dl_x", "0");

	strcpy(pool, nvram_safe_get("apps_pool"));
	strcpy(share, nvram_safe_get("apps_share"));
	strcpy(rundl, nvram_safe_get("apps_dlx"));
	strcpy(rundms, nvram_safe_get("apps_dmsx"));
	strcpy(name, nvram_safe_get("computer_name"));
	strcpy(mymac, nvram_safe_get("et0macaddr"));
	
	strcpy(share_port_from, nvram_get("apps_dl_share_port_from"));
	strcpy(share_port_to, nvram_get("apps_dl_share_port_to"));
	fp=fopen("/tmp/.prange", "w");
	if(fp)
	{
		fprintf(fp,"%s:%s", share_port_from, share_port_to);
		fclose(fp);
	}

	if (nvram_match("apps_dmsx", "1"))
	{
//		sprintf(tmpstr2, "%s/dms /shares/DMSRoot \"%s\" %s &", "/apps/bin", name, mymac);
//		ret=system(tmpstr2);
//		if(!ret){
//			nvram_set("dms_running", "1");	// 2007.11 James.
//			logmessage("Media Server", "daemon is started");
//		}

		write_dms_conf();

		ret = system("/usr/bin/ushare -D");

		if (!ret)
		{
			nvram_set("dms_running", "1");
			logmessage("Media Server", "daemon is started");
		}

	}
	if (nvram_match("apps_dlx", "1")/* && nvram_match("swap_on", "1")*/ && nvram_match("apps_comp", "1"))
	{
		sprintf(tmpstr2, "%s/dmex", EXBIN);
		ret=system(tmpstr2);
		nvram_set("apps_running", "1");
		if(!ret)
		{
			nvram_set("apps_dl_x", "1");
			logmessage("Download Master", "daemon is started");
		}
	}
	
	nvram_set("apps_installed", "1");
}

void start_apps()			// added by Jiahao for WL500gP
{
	printf("\npreparing to start Download Machine\n");
	char pool[32];
	char share[32];
	char origver[32];
	char target_dir[32];
	char pool_dir[32];
	char EXLIB[32]="/.apps/lib";
	char EXBIN[32]="/.apps/bin";
	char EXUSR[32]="/.apps/usr";
	char EXETC[32];
	char NEWVER[8];
	char tmpstr[256];
	char tmpstr2[256];
	char tmpstr3[256];
	char tmpstr4[256];

	char test_path[128];
	char *p;

	if ( nvram_match("apps_dms_usb_port_x", nvram_safe_get("usb_disc0_port")) )
	{
		strcpy(test_path, nvram_safe_get("usb_disc0_path0"));
		p=strstr(test_path, "/tmp/harddisk");
		p+=5;
		nvram_set("apps_pool", p);
	}
	else if ( nvram_match("apps_dms_usb_port_x", nvram_safe_get("usb_disc1_port")) )
	{
		strcpy(test_path, nvram_safe_get("usb_disc1_path0"));
		p=strstr(test_path, "/tmp/harddisk");
		p+=5;
		nvram_set("apps_pool", p);
	}
	
	strcpy(tmpstr, EXLIB);
	sprintf(EXLIB, "%s%s", test_path, tmpstr);
	strcpy(tmpstr, EXBIN);
	sprintf(EXBIN, "%s%s", test_path, tmpstr);
	strcpy(tmpstr, EXUSR);
	sprintf(EXUSR, "%s%s", test_path, tmpstr);

	//strcpy(pool, nvram_get("apps_pool"));
	strcpy(pool, p);	// 2008.05 James.
	strcpy(share, nvram_get("apps_share"));
	strcpy(origver, nvram_get("apps_ver"));
	sprintf(target_dir, "/shares/%s/%s", pool, share);
	sprintf(pool_dir, "/shares/%s", pool);
	sprintf(EXETC, "%s/.etc", pool_dir);

	eval("rm", "-rf", "/shares/lib");
	eval("rm", "-rf", "/shares/bin");
	eval("rm", "-rf", "/shares/usr");
	eval("rm", "-rf", "/shares/etc");
	eval("rm", "-rf", EXETC);

	mkdir_if_none(pool_dir);
	mkdir_if_none(target_dir);

	eval("ln", "-s", EXLIB, "/shares/lib");
	eval("ln", "-s", EXBIN, "/shares/bin");
	eval("ln", "-s", EXUSR, "/shares/usr");
	
	set_apps_ver();
	sprintf(NEWVER, nvram_safe_get("apps_ver"));

	sprintf(tmpstr, "%s/Music", target_dir);
	if (mkdir_if_none(tmpstr)==0)
		printf("Music DIR exist\n");

	sprintf(tmpstr, "%s/Video", target_dir);
	if (mkdir_if_none(tmpstr)==0)
		printf("Video DIR exist\n");

	sprintf(tmpstr, "%s/Photo", target_dir);
	if (mkdir_if_none(tmpstr)==0)
		printf("PHOTO DIR exist\n");

	sprintf(tmpstr, "%s/Download", target_dir);
	if (mkdir_if_none(tmpstr)==0)
		printf("DOWNLOAD DIR exist\n");

	sprintf(tmpstr, "%s/Download/config", target_dir);
	mkdir_if_none(tmpstr);
	sprintf(tmpstr, "%s/Download/Complete", target_dir);
	mkdir_if_none(tmpstr);
	sprintf(tmpstr, "%s/Download/InComplete", target_dir);
	mkdir_if_none(tmpstr);
	sprintf(tmpstr, "%s/Download/.logs", target_dir);
	mkdir_if_none(tmpstr);	

	eval("rm", "-rf", EXETC);
	mkdir_if_none(EXETC);
	eval("ln", "-s", EXETC, "/shares/etc");
	
	if (nvram_match("apps_dlx", "1") /*&& nvram_match("swap_on", "1")*/ && nvram_match("apps_comp", "1"))
	{
		sprintf(tmpstr, "%s/asus_gift.conf", EXETC);
		if (check_if_file_exist(tmpstr)==1)
			printf("FILE asus_gift exist\n");
		else
		{
			sprintf(tmpstr, "%s/gift-nasoc/bin/asus_gift.conf", EXUSR);
			sprintf(tmpstr2, "cp %s %s/. -f", tmpstr, EXETC);
			system(tmpstr2);
		}
		
		sprintf(tmpstr, "%s/.giFT", EXETC);
		if (mkdir_if_none(tmpstr)==0)
			printf(".giFT DIR exist\n");
		else
		{
			sprintf(tmpstr2, "%s/.", tmpstr);
			sprintf(tmpstr4, "cp /shares/usr/.giFT/* %s -f", tmpstr2);
			system(tmpstr4);
			
			sprintf(tmpstr2, "%s/FastTrack", tmpstr);
			mkdir_if_none(tmpstr2);
			
			sprintf(tmpstr3, "%s/.", tmpstr2);
			sprintf(tmpstr4, "cp /shares/usr/.giFT/FastTrack/* %s -f", tmpstr3);
			system(tmpstr4);
			
			sprintf(tmpstr2, "%s/OpenFT", tmpstr);
			mkdir_if_none(tmpstr2);
			
			sprintf(tmpstr3, "%s/.", tmpstr2);
			sprintf(tmpstr4, "cp /shares/usr/.giFT/OpenFT/* %s -f", tmpstr3);
			system(tmpstr4);
			
			sprintf(tmpstr2, "%s/Gnutella", tmpstr);
			mkdir_if_none(tmpstr2);
			
			sprintf(tmpstr3, "%s/.", tmpstr2);
			sprintf(tmpstr4, "cp /shares/usr/.giFT/Gnutella/* %s -f", tmpstr3);
			system(tmpstr4);
			
			sprintf(tmpstr2, "%s/completed", tmpstr);
			mkdir_if_none(tmpstr2);
			sprintf(tmpstr2, "%s/completed/corrupted", tmpstr);
			mkdir_if_none(tmpstr2);
			sprintf(tmpstr2, "%s/ui", tmpstr);
			mkdir_if_none(tmpstr2);
			
			sprintf(tmpstr3, "%s/.", tmpstr2);
			sprintf(tmpstr4, "cp /shares/usr/.giFT/ui/* %s -f", tmpstr3);
			system(tmpstr4);
			
			sprintf(tmpstr, "%s/giFT", EXETC);
			mkdir_if_none(tmpstr);
			
			sprintf(tmpstr2, "%s/.", tmpstr);
			sprintf(tmpstr4, "cp /shares/usr/gift-nasoc/share/giFT/* %s -f", tmpstr2);
			system(tmpstr4);
			
			sprintf(tmpstr2, "%s/FastTrack", tmpstr);
			mkdir_if_none(tmpstr2);
			sprintf(tmpstr2, "%s/OpenFT", tmpstr);
			mkdir_if_none(tmpstr2);		
			sprintf(tmpstr2, "%s/Gnutella", tmpstr);
			mkdir_if_none(tmpstr2);
			sprintf(tmpstr2, "%s/completed", tmpstr);
			mkdir_if_none(tmpstr2);
			sprintf(tmpstr2, "%s/completed/corrupted", tmpstr);
			mkdir_if_none(tmpstr2);
			sprintf(tmpstr2, "%s/ui", tmpstr);
			mkdir_if_none(tmpstr2);
			
			sprintf(tmpstr3, "%s/.", tmpstr2);
			sprintf(tmpstr4, "cp /shares/usr/gift-nasoc/share/giFT/ui/* %s -f", tmpstr3);
			system(tmpstr4);
		}
		
		eval("rm" ,"-rf" ,"/shares/dmathined");
		mkdir_if_none("/shares/dmathined");
		mkdir_if_none("/shares/dmathined/Download");
		
		sprintf(tmpstr, "%s/Download/config", target_dir);
		eval("ln", "-s", tmpstr, "/shares/dmathined/Download/config");
		sprintf(tmpstr, "%s/Download/Complete", target_dir);
		eval("ln", "-s", tmpstr, "/shares/dmathined/Download/Complete");
		sprintf(tmpstr, "%s/Download/InComplete", target_dir);
		eval("ln", "-s", tmpstr, "/shares/dmathined/Download/InComplete");
		sprintf(tmpstr, "%s/Download/.logs", target_dir);
		eval("ln", "-s", tmpstr, "/shares/dmathined/Download/.logs");
	}
	
	eval("rm" ,"-rf" ,"/shares/DMSRoot");
	mkdir_if_none("/shares/DMSRoot");
	
	sprintf(tmpstr, "%s/Download/Complete", target_dir);
	eval("ln", "-s", tmpstr, "/shares/DMSRoot/Download");
	sprintf(tmpstr, "%s/Video", target_dir);
	eval("ln", "-s", tmpstr, "/shares/DMSRoot/Video");
	sprintf(tmpstr, "%s/Music", target_dir);
	eval("ln", "-s", tmpstr, "/shares/DMSRoot/Music");
	sprintf(tmpstr, "%s/Photo", target_dir);
	eval("ln", "-s", tmpstr, "/shares/DMSRoot/Photo");
	
	eval("/sbin/test_of_var_files_in_mount_path", pool_dir);	// 2008.05 James.

	// Change mode
	chmod ("/tmp/harddisk/part0/.apps/bin/*", 0777);
	chmod ("/tmp/harddisk/part0/.apps/lib/*", 0777);
	chmod ("/tmp/harddisk/part0/.apps/usr/gift-nasoc/bin/*", 0777);
	chmod ("/tmp/harddisk/part0/.apps/usr/gift-nasoc/lib/*", 0777);

	chmod ("/tmp/harddisk/part0/share", 0777);
	chmod ("/tmp/harddisk/part0/share/Download", 0777);
	chmod ("/tmp/harddisk/part0/share/Download/Complete", 0777);
	chmod ("/tmp/harddisk/part0/share/Download/InComplete", 0777);

	exec_apps();
	
}

void get_first_partition(char *part_path)		// added by Jiahao for WL500gP
{
	strcpy(part_path, "");
	if (nvram_match("usb_disc0_dev", ""))
	{
		if (nvram_invmatch("usb_disc1_dev", ""))
		{
			if (nvram_invmatch("usb_disc1_index", "0"))
			{
				strcpy(part_path, nvram_safe_get("usb_disc1_path0"));
			}
		}
	}
	else
	{
		if (nvram_invmatch("usb_disc0_index", "0"))
		{
			strcpy(part_path, nvram_safe_get("usb_disc0_path0"));
		}
	}
}

// 2007.10 James {
void start_usb_apps(){
printf("*** start_usb_apps() ***\n");
	run_ftpsamba();
	start_apps();
}

void stop_usb_apps(){
printf("*** stop_usb_apps() ***\n");
	stop_apps();
	stop_ftpsamba();
	sleep(1);
}

void stop_apps(){
	eval("killall", "snarf");
	eval("killall", "ctorrent");
	eval("killall", "rtorrent");
	eval("killall", "giftd");
	eval("killall", "dmathined");
	
	stop_dms();
	
	nvram_set("apps_running", "0");
	nvram_set("apps_dl_x", "0");
	sleep(1);
}

void stop_ftpsamba(){
	stop_ftp();
	stop_samba();
}

void stop_ftp(){
	if(nvram_match("ftp_running", "0"))
		return ;
	
	fprintf(stderr, "Stoping vsftpd\n");
	
	eval("killall", "-SIGKILL", "vsftpd");
	eval("rm", "-f", "/tmp/vsftpd.conf");
	
	logmessage("FTP Server", "daemon is stoped");
	
	nvram_set("ftp_running", "0");
}

void stop_samba(){
	if(nvram_match("samba_running", "0"))
		return ;
	
	fprintf(stderr, "Stoping samba\n");
	
	eval("killall", "-SIGKILL", "smbd");
	eval("killall", "-SIGKILL", "nmbd");
	eval("rm", "-f", "/tmp/samba/lib/smb.conf");
	
	logmessage("Samba Server", "daemon is stoped");
	
	nvram_set("samba_running", "0");
	nvram_set("st_samba_mode_x", "0");	// 2007.11 James.
}

void stop_dms(){
	if(nvram_invmatch("dms_running", "1"))
		return;
	
	fprintf(stderr, "Stoping dms\n");
	
//	eval("killall", "dms");
	eval("killall", "ushare");
	
	logmessage("Media Server", "daemon is stoped");
	
	nvram_set("dms_running", "0");
}

int run_dms(){
	if(nvram_match("dms_running", "0")){
		if(start_dms() == 0){
			nvram_set("dms_running", "1");
		}
		
		return -1;
	}
	
	return 0;
}

void
write_dms_conf()
{
	FILE *fp;

	/* write /etc/ushare.conf */

	fp = fopen("/tmp/ushare.conf", "r");

	if (fp)		// file existed
	{
		fclose(fp);
		return;
	}

	fp = fopen("/tmp/ushare.conf", "w");

	if (fp == NULL)
		return;

	fprintf(fp, "USHARE_NAME=%s\n", nvram_safe_get("computer_name"));	// default is 'uShare'
	fprintf(fp, "USHARE_IFACE=%s\n", "br0");				// default is eth0
	fprintf(fp, "USHARE_PORT=%s\n", "");
	fprintf(fp, "USHARE_TELNET_PORT=%s\n", "");
//	fprintf(fp, "USHARE_DIR=%s,%s,%s\n",  "/shares/DMSRoot/Music",          // Directories to be shared
//						"/shares/DMSRoot/Photo",
//                                              "/shares/DMSRoot/Video");
	fprintf(fp, "USHARE_DIR=%s\n",  "/shares/DMSRoot");
	fprintf(fp, "USHARE_OVERRIDE_ICONV_ERR=%s\n", "1");                     // adds the non-iconv'ed string into the media list
	fprintf(fp, "USHARE_ENABLE_WEB=%s\n", "no");
	fprintf(fp, "USHARE_ENABLE_TELNET=%s\n", "no");
	fprintf(fp, "USHARE_ENABLE_XBOX=%s\n", "yes");				// Use XboX 360 compatibility mode
	fprintf(fp, "USHARE_ENABLE_DLNA=%s\n\n", "no");				// Use DLNA profile
										// This is needed for PlayStation3 to work (among other devices)
	fclose(fp);
}

int
start_dms()
{
	int ret = 0;

	if(nvram_invmatch("apps_dmsx", "1"))
		return;

	mkdir_if_none("/shares/DMSRoot");
	write_dms_conf();

	ret = system("/usr/bin/ushare -D");
	if (!ret)
	{
		nvram_set("dms_running", "1");
		logmessage("Media Server", "daemon is started");

		return 0;
	}

	return 1;
}

#if 0
int start_dms(){
	char *DMS_ROOT = "/shares/DMSRoot";
	char name[32];
	char mymac[32];
	char path[256];
	char tmpstr2[256];
	int ret = 0;
	pid_t pid;
	char *argvs[5];
	
	if(nvram_invmatch("apps_dmsx", "1"))
		return;
	
	mkdir_if_none(DMS_ROOT);
	
	strcpy(name, nvram_safe_get("computer_name"));
	strcpy(mymac, nvram_safe_get("et0macaddr"));
	sprintf(tmpstr2, "/apps/bin/dms %s \"%s\" %s &", DMS_ROOT, name, mymac);
	
	ret = system(tmpstr2);
	if(!ret){
		nvram_set("dms_running", "1");	// 2007.11 James.
		logmessage("Media Server", "daemon is started");
		
		return 0;
	}
	
	return 1;
}
#endif

void
run_ftpsamba()
{
	run_ftp();
	run_samba();
}

void
run_samba()
{
	if(nvram_match("samba_running", "0"))
	{
		//if (nvram_invmatch("st_samba_modex", "0"))
		if(nvram_invmatch("st_samba_mode", "0"))	// 2007.11 James.
		{
			fprintf(stderr, "starting samba\n");
			if (start_samba()==0)
			{
				nvram_set("samba_running", "1");
			}
		}
	}
	else
	{
		//if (nvram_invmatch("st_samba_modex", "0"))
		if(nvram_invmatch("st_samba_mode", "0"))	// 2007.11 James.
		{
// 2007.11 James. For new /tmp/passwd and new account. {
			//fprintf(stderr, "rewrite conf of samba\n");
			//write_smb_conf();
			fprintf(stderr, "restarting samba\n");
			if(!start_samba())
				nvram_set("samba_running", "1");
// 2007.11 James. For new /tmp/passwd and new account. }
		}
	}
}

void
run_ftp()
{
	if(nvram_match("ftp_running", "0"))
	{
		//if (nvram_invmatch("st_ftp_modex", "0"))
		if (nvram_invmatch("st_ftp_mode", "0"))	// 2007.10 James.
		{
			fprintf(stderr, "starting vsftpd\n");
			if (start_ftpd()==0)
			{
				nvram_set("ftp_running", "1");
			}
		}
	}
	else
	{
		//if (nvram_invmatch("st_ftp_modex", "0"))
		if (nvram_invmatch("st_ftp_mode", "0"))	// 2007.10 James.
		{
			//fprintf(stderr, "rewrite conf of vsftpd\n");
			//write_ftpd_conf();
			fprintf(stderr, "restarting ftp\n");
			if(!start_ftpd())
				nvram_set("ftp_running", "1");
		}
	}
}

int
run_apps()
{
	int apps_comp=0;
	int apps_comp_pre=0;
	int apps_status=0;
	int apps_disk_free=0;
	int ret=0, i;
	char tmp[32];
	char tmpstr[64];
	char tmpstr2[64];
	FILE *fp;
	char *buf=NULL;
	int buflen=0;
	struct sysinfo info;
	int fd=0;

	nvram_set("usb_storage_busy", "1");
	nvram_set("apps_status_checked", "1");

	nvram_set("apps_dlx", nvram_safe_get("apps_dl"));

	printf("Starting download master...\n");

	memset(tmp, 0, sizeof(tmp));
	if ( nvram_invmatch("usb_disc0_dev", "") )
	{
		strcpy(tmp, nvram_safe_get("usb_disc0_path0"));
	}
	else if ( nvram_invmatch("usb_disc1_dev", "") )
	{
		strcpy(tmp, nvram_safe_get("usb_disc1_path0"));
	}

	apps_disk_free=check_disk_free_apps(tmp, apps_comp);

	if (apps_disk_free==1)
		nvram_set("apps_disk_free", "1");
	else
		nvram_set("apps_disk_free", "0");

	apps_comp = ckeck_apps_completeness(tmp);
	if(apps_comp == 1)
		nvram_set("apps_comp", "1");
	else{
		nvram_set("apps_comp", "0");
	}

	if(!nvram_match("apps_running", "1") &&
		(!nvram_match("dm_block", "1")) &&
		((apps_comp_pre==1 && nvram_match("apps_dlx", "1")) || nvram_match("apps_dmsx", "1"))
	){
		if(strcmp(tmp, "") != 0){


			if ( //nvram_match("apps_dlx", "0") ||
			    (nvram_match("apps_dlx", "1") && /*nvram_match("swap_on", "1") &&*/ check_disk_free_GE_1G(tmp)==2) ||
			    (nvram_match("apps_dmsx", "1") && !(nvram_match("apps_dlx", "1") && nvram_match("swap_on", "1") && check_disk_free_GE_1G(tmp)==2))
			   )
			{
//				if (apps_comp==1 && apps_disk_free==1)
				if ( (apps_comp==1 && apps_disk_free==1) || nvram_match("apps_dmsx", "1"))
				{
					if (strcmp(tmp, nvram_safe_get("usb_disc0_path0"))==0)
						nvram_set("apps_dms_usb_port_x", nvram_safe_get("usb_disc0_port"));
					else if (strcmp(tmp, nvram_safe_get("usb_disc1_path0"))==0)
						nvram_set("apps_dms_usb_port_x", nvram_safe_get("usb_disc1_port"));

					start_apps();
//					nvram_set("apps_running", "1");
//					nvram_set("apps_status_checked", "1");
				}

				if (nvram_match("swap_on", "0") && apps_comp == 1)
				{
					if (strcmp(tmp, "")!=0)
					{
						memset(tmpstr, 0, sizeof(tmpstr));
						sprintf(tmpstr, "%s/swap", tmp);
						unlink(tmpstr);
						memset(tmpstr2, 0, sizeof(tmpstr));
						sprintf(tmpstr2, "%s/.swap", tmp);
						unlink(tmpstr2);

						ret = check_disk_free_GE_1G(tmp);
						if (ret==2)
						{
							if(!strcmp(tmp, nvram_safe_get("usb_disc0_path0")))
								nvram_set("apps_dms_usb_port_x2", nvram_safe_get("usb_disc0_port"));
							else if (!strcmp(tmp, nvram_safe_get("usb_disc1_path0")))
								nvram_set("apps_dms_usb_port_x2", nvram_safe_get("usb_disc1_port"));
							
							sysinfo(&info);

							if ((info.freeram + info.bufferram) >= 1024*1024*2)
								buflen = 1024*1024*2;
							else if ((info.freeram + info.bufferram) >= 1024*1024*1.6)
								buflen = 1024*1024*1.6;
							else if ((info.freeram + info.bufferram) >= 1024*1024*1)
								buflen = 1024*1024*1;
							else if ((info.freeram + info.bufferram) >= 1024*1024*0.8)
								buflen = 1024*1024*0.8;
							else
								buflen = 1024*1024*0.5;

							buf = malloc(buflen);
							for(i=0;i<buflen;i++)
								buf[i]='\n';

							int count = 1024*1024*64/buflen;
		
							fp=fopen(tmpstr, "w");
							if(fp!=NULL)
							{
								for(i=0;i<count;i++){
									fprintf(fp, "%s", buf);
								}
								free(buf);
								fclose(fp);

								eval("cp", tmpstr, tmpstr2);
								eval("rm", "-rf", tmpstr);

								eval("/sbin/mkswap", tmpstr2);

								swapon(tmpstr2, 0);	// 2008.04 James.

								logmessage("USB storage", "64MB swap file is added in run_apps");
								nvram_set("swap_on", "1");
								nvram_set("swap_file", tmpstr2);	// 2008.06 James.
							}

						}
						else if (ret==1)
							logmessage("USB storage", "The swap file is not added for free space is less than 64MB");
						else if (ret==3)
							logmessage("USB storage", "The swap file is not added for partition size is less than 1024MB");
						else if (ret==0)
							logmessage("USB storage", "The swap file is not added for unknown reasons");
					}
				}

			}
			else
			{
				fprintf(stderr, "skip running DMS & DM for no swap file or binary completeness issues\n");
				logmessage("Media Server", "daemon is not started for no swap file or binary completeness issues");
				logmessage("Download Master", "daemon is not started for no swap file or binary completeness issues");
			}
		}

		if ((apps_comp == 1) ||(apps_comp_pre == 1))
			nvram_set("apps_status_checked", "0"); // Files checked
		else
			nvram_set("apps_status_checked", "1"); // Need to check

	}	
	else
		fprintf(stderr, "skip running DMS & DM\n");

	nvram_set("usb_storage_busy", "0");
	
	return 0;
}

int
swap_check()
{
	struct sysinfo info;
	
	sysinfo(&info);
	if(info.totalswap > 0)
		return 1;
	else
		return 0;
}
#endif

int
umount_disc_parts_rc(int usb_port)
{	
	char nvramstr[32];
	char nvramstr2[32];
	char nvramstr3[32];
	char umount_dir[32];
	int disc_index, i;
	char tmp[2];
	int dev_index=-1;
	int count=0;
	char test_path[128], tmpstr[128];
	int ret=0;
#ifdef DLM
	int sleep_flag=0;

	//if ( strcmp(nvram_safe_get("usb_storage_busy"), "1" )==0 )	// 2007.12 James
	//	return;
#endif
	
	sprintf(tmp, "%d", usb_port);
	if (strcmp(nvram_safe_get("usb_disc0_port"), tmp)==0)
		dev_index=0;
	else if (strcmp(nvram_safe_get("usb_disc1_port"), tmp)==0)
		dev_index=1;
	else
		return -1;
	
#ifdef DLM
#if 1
	if ( (strcmp(nvram_safe_get("apps_running"), "1")==0) && (atoi(nvram_safe_get("apps_dms_usb_port_x"))==usb_port) )
	{
		sleep_flag=1;

		nvram_set("dm_block", "1");

		eval("killall", "snarf");
		eval("killall", "ctorrent");
		eval("killall", "rtorrent");
		eval("killall", "giftd");
		eval("killall", "dmathined");
		//eval("killall", "dms");

		//nvram_set("dms_running", "0");	// 2007.11 James.
		nvram_set("apps_running", "0");
		nvram_set("apps_installed", "0");
		//nvram_set("apps_dl_x", "0");
		nvram_set("apps_dlx", "0");
		nvram_set("apps_status_checked", "0");
		nvram_set("apps_disk_free", "0");
		/* patch for DM status */
		nvram_set("apps_comp", "0");

		/*nvram_set("usb_storage_busy2", "1");
		sleep(5);
		nvram_set("usb_storage_busy2", "0");*/
	}
#endif
	if(!strcmp(nvram_safe_get("samba_running"), "1")){
		eval("killall", "nmbd");
		eval("killall", "smbd");
		nvram_set("samba_running", "0");
		nvram_set("st_samba_mode_x", "0");
	}
	
	if(!strcmp(nvram_safe_get("ftp_running"), "1")){
		eval("killall", "vsftpd");
		nvram_set("ftp_running", "0");
	}
	
// 2007.11 James. {
	if(!strcmp(nvram_safe_get("dms_running"), "1")){
//		eval("killall", "dms");
		eval("killall", "ushare");
		nvram_set("dms_running", "0");
	}
// 2007.11 James. }
	
	if (sleep_flag==1)
	{
		nvram_set("usb_storage_busy2", "1");
		for(i = 1; i <= 5; ++i){
			printf("Sleep %d second%s...\n", i, ((i == 1)?"":"s"));
			sleep(1);
		}
		nvram_set("usb_storage_busy2", "0");
	}//*/
	sync();	// 2008.06 James.
	
	if(!strcmp(nvram_safe_get("swap_on"), "1") && atoi(nvram_safe_get("apps_dms_usb_port_x2")) == usb_port)
	{
		strcpy(test_path, "");
		if ( strcmp(nvram_safe_get("usb_disc0_port"), nvram_safe_get("apps_dms_usb_port_x2"))==0 )
		{
			strcpy(test_path, nvram_safe_get("usb_disc0_path0"));
		}
		else if ( strcmp(nvram_safe_get("usb_disc1_port"), nvram_safe_get("apps_dms_usb_port_x2"))==0 )
		{
			strcpy(test_path, nvram_safe_get("usb_disc1_path0"));
		}
		
		if(strcmp(test_path, "") != 0)
		{
			sprintf(tmpstr, "%s/.swap", test_path);
			printf("try to swapoff %s.\n", tmpstr);
			swapoff(tmpstr);
			
			//nvram_set("usb_storage_busy2", "1");
			sleep(1);
			//nvram_set("usb_storage_busy2", "0");
			
			if (swap_check()==1)
			{
				logmessage("USB storage", "swapoff unsuccessfully");
				nvram_set("swapoff_failed", "1");
				//unlink(tmpstr);
				return -1;
			}
			else
			{
				logmessage("USB storage", "swapoff successfully");
				nvram_set("swap_on", "0");
				nvram_set("swap_file", "");	// 2008.06 James.
				nvram_set("swapoff_failed", "0");
				nvram_set("apps_dms_usb_port_x2", "-1");
				unlink(tmpstr);
			}
		}
	}
#endif

	sprintf(nvramstr, "usb_disc%d_index", dev_index);
	disc_index=atoi(nvram_safe_get(nvramstr));
	
	for (i=disc_index; i>0; i--)
	{
		sprintf(nvramstr2, "usb_disc%d_path%d", dev_index, i-1);
		sprintf(umount_dir, nvram_safe_get(nvramstr2));
		sprintf(nvramstr3, "usb_disc%d_fs_path%d", dev_index, i-1);
		count=0;
// 2009.0623 James. {
		//while((ret=umount(umount_dir)) && (count<2))
		while((ret=umount2(umount_dir, MS_NOEXEC|MS_NOSUID|0x00000002)) && (count<2))	// 0x00000002: MNT_DETACH
// 2009.0623 James. {
		{
			//sync();	// 2009.0623 James.
			count++;
#ifdef DLM
			nvram_set("usb_storage_busy2", "1");
#endif
			sleep(1);
#ifdef DLM
			nvram_set("usb_storage_busy2", "0");
#endif
		}
		if(!ret)
		{
			rmdir(umount_dir);
			nvram_set(nvramstr3, "");
			nvram_set(nvramstr2, "");
			sprintf(tmp, "%d", i-1);
			nvram_set(nvramstr, tmp);
			sprintf(tmpstr, "unmounted %s", umount_dir);
			logmessage("USB storage", tmpstr);
			if (strcmp(umount_dir, "/tmp/harddisk/part0")==0)
				nvram_set("mnt_type", "");
		}
		else
			return ret;
	}
// 2009.0623 James. {
	memset(nvramstr, 0, sizeof(nvramstr));
	sprintf(nvramstr, "usb_disc%d_pid", dev_index);
	nvram_set(nvramstr, "");
	sprintf(nvramstr, "usb_disc%d_safely_removed", dev_index);
	nvram_set(nvramstr, "");
	memset(nvramstr, 0, sizeof(nvramstr));
// 2009.0623 James. {
// Move to eject devices
	if (nvram_match("apps_dl_x","0")) {
		sprintf(nvramstr, "usb_disc%d_dev", dev_index);
		nvram_set(nvramstr, "");
		sprintf(nvramstr, "usb_disc%d_port", dev_index);
		nvram_set(nvramstr, "0");
	}

#ifdef DLM
	nvram_set("eject_from_web", "1");
#endif
	//write_disc_status();
	
	return 0;
}

#ifdef USB_SUPPORT
/* check if the block device has no partition */
int
is_no_partition(const char *discname)   
{
        FILE *procpt;
        char line[128], ptname[32];
        int ma, mi, sz;
        int count = 0;

        if (procpt = fopen_or_warn("/proc/partitions", "r"))
        {
                while (fgets(line, sizeof(line), procpt))
                {
                        if (sscanf(line, " %d %d %d %[^\n ]", &ma, &mi, &sz, ptname) != 4)
                                continue;
                        if (strstr(ptname, discname))
                                count++;
                }
        }

        if (count == 1)
                return 1;
        else
                return 0;
}
#endif


//2008.07.17 Yau add
int start_networkmap(void)
{
	char *networkmap_argv[] = {"networkmap", NULL};
	pid_t pid;
	
	_eval(networkmap_argv, NULL, 0, &pid);
	
	return 0;
}

// 2009.10.22 Yen
void start_wps_proc (int mode)
{

	char nvifname[IFNAMSIZ];
        char osifname[IFNAMSIZ];
        char pin[9] = {0};
        char action_str[16];

        char uibuf[256] = "SET ";
        int uilen = 4;

        /* Set variables for WPS te generate configation when AP is in un-configured mode */

//	printf("dbg: start WPS \n");
	if (mode == WPS_UI_METHOD_PBC) {
		nvram_set("wps_sta_pin", "00000000");
	        if (nvram_match("wps_config_state", "0") && !nvram_match("wps_asus_mode","1")) {

//			printf("dbg: wps_config_state = 0, set the variables \n");

	        	nvram_set("wps_unconfig_addenr", "1");
	                nvram_set("wl_wps_reg", "enabled");
        	        nvram_set("wl0_wps_reg", "enabled");
                	nvram_set("wl_wps_config_state", "1");
	                nvram_set("wl0_wps_config_state", "1");
        	        nvram_set("wps_config_state", "1");
	        }

        	if (nvram_match("wps_unconfig_addenr", "0")) {

//			printf("dbg: wps_unconfig_addenr = 0\n");

	        	nvram_set("is_default", "0");
        	        nvram_set("is_modified", "1");
	       	}

        	if (nvram_match("wl_wps_reg", "disabled")) {
//			printf("dbg: wl_wps_reg = disabled \n");
	                nvram_set("wl_wps_config_state", "1");
        	        nvram_set("wl0_wps_config_state", "1");
                	nvram_set("wl_wps_reg", "enabled");
	                nvram_set("wl0_wps_reg", "enabled");
		}
	}
	else if (mode == WPS_UI_METHOD_PIN) {
		if (nvram_match("wps_sta_pin", "00000000") || nvram_match("wps_sta_pin","") || (nvram_match("wps_config_state", "0"))) {
//			nvram_set("wps_sta_pin","");
//			nvram_set("wl_wps_reg","disabled");
			nvram_set("wl_wps_reg","enabled");
			nvram_set("wl0_wps_reg","enabled");
			nvram_set ("wps_uncofnig_addenr", "1");
		}
		else {
			nvram_set("wl_wps_reg","disabled");
			nvram_set("wl0_wps_reg","disabled");
			nvram_set ("wps_uncofnig_addenr", "0");
		}
		
//		if (nvram_match("wps_config_state", "0") && nvram_invmatch("wps_sta_pin","")) {
/*		if (nvram_match("wps_config_state", "0")) {
			printf("dbg: set the unconfig addenr\n");
			nvram_set ("wps_uncofnig_addenr", "1");
			nvram_set("wl0_wps_reg","enabled");
		}
*/
	}

        /* Use push button to connect client*/
	if (mode == WPS_UI_METHOD_PBC) {
                if (!nvram_match("wps_asus_mode", "1")) {
        		uilen += sprintf(uibuf + uilen, "wps_method=%d ", WPS_UI_METHOD_PBC);
                        uilen += sprintf(uibuf + uilen, "wps_action=%d ", WPS_UI_ACT_ADDENROLLEE);
                        uilen += sprintf(uibuf + uilen, "wps_sta_pin=00000000 ");
                }
                else { // Special for ASUS mode
			nvram_set("wps_asus_mode", "0");
        		uilen += sprintf(uibuf + uilen, "wps_method=%d ", WPS_UI_METHOD_PIN);
                        uilen += sprintf(uibuf + uilen, "wps_action=%d ", WPS_UI_ACT_CONFIGAP);
//                        uilen += sprintf(uibuf + uilen, "wps_device_pin=00000000 ");
                }
/*
		uilen += sprintf(uibuf + uilen, "wps_sta_pin=00000000 ");
	        uilen += sprintf(uibuf + uilen, "wps_action=%d ", WPS_UI_ACT_ADDENROLLEE);
*/
	} else if (mode == WPS_UI_METHOD_PIN) {
        	uilen += sprintf(uibuf + uilen, "wps_method=%d ", WPS_UI_METHOD_PIN);
		if (nvram_invmatch("wps_sta_pin","") || nvram_match("wps_sta_pin", "00000000"))  { /* Add Enrollee */
			uilen += sprintf(uibuf + uilen, "wps_sta_pin=%s ", nvram_get("wps_sta_pin"));
		        uilen += sprintf(uibuf + uilen, "wps_action=%d ", WPS_UI_ACT_ADDENROLLEE);
		}
		else {
			uilen += sprintf(uibuf + uilen, "wps_action=%d ", WPS_UI_ACT_CONFIGAP);
		}
	}

        nvram_set("wps_proc_status", "0");
        strcpy(wps_unit, nvram_safe_get("wl_unit"));

        uilen += sprintf(uibuf + uilen, "wps_config_command=%d ", WPS_UI_CMD_START);
        sprintf(nvifname, "wl%s", wps_unit);
        nvifname_to_osifname(nvifname, osifname, sizeof(osifname));

        uilen += sprintf(uibuf + uilen, "wps_pbc_method=%d ", WPS_UI_PBC_SW);
        uilen += sprintf(uibuf + uilen, "wps_ifname=%s ", osifname);
//	printf("dbg: wps ended, uibuf = %s\n", uibuf);
        set_wps_env(uibuf);
}

void stop_wps_proc (void)
{

	char nvifname[IFNAMSIZ];
        char osifname[IFNAMSIZ];
        char pin[9] = {0};
        char action_str[16];

        char uibuf[256] = "SET ";
        int uilen = 4;

        /* Set variables for WPS te generate configation when AP is in un-configured mode */

	printf("dbg: stop WPS \n");

        wps_action = 0;

        uilen += sprintf(uibuf + uilen, "wps_config_command=%d ", WPS_UI_CMD_STOP);
        uilen += sprintf(uibuf + uilen, "wps_action=%d ", WPS_UI_ACT_NONE);
        printf("dbg: WPS uibuf = %s\n", uibuf);
        set_wps_env(uibuf);

//        action = NOTHING;

//	printf("dbg:Stop wps ended, uibuf = %s\n", uibuf);
}

int restart_wps_proc(void)
{
	char *wps_argv[] = {"/bin/wps_monitor", NULL};
	pid_t pid;
	eval("killall","wps_monitor");
//        eval("killall","wps_ap");
//        eval("killall","wps_enr");
        _eval(wps_argv, NULL, 0, &pid);
	return 0;
}


static int
write_to_wps(int fd, char *cmd)
{
        int n;
        int len;
        struct sockaddr_in to;

        len = strlen(cmd)+1;

        /* open loopback socket to communicate with wps */
        memset(&to, 0, sizeof(to));
        to.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
        to.sin_family = AF_INET;
        to.sin_port = htons(WPS_UI_PORT);

        n = sendto(fd, cmd, len, 0, (struct sockaddr *)&to,
                sizeof(struct sockaddr_in));

        /* Sleep 100 ms to make sure
 *  *            WPS have received socket */
        USLEEP(100*1000);
        return n;
}

static int
read_from_wps(int fd, char *databuf, int datalen)
{
        int n, max_fd = -1;
        fd_set fdvar;
        struct timeval timeout;
        int recvBytes;
        struct sockaddr_in addr;
        socklen_t size = sizeof(struct sockaddr);

        timeout.tv_sec = 2;
        timeout.tv_usec = 0;

        FD_ZERO(&fdvar);

        /* get ui fd */
        if (fd >= 0) {
                FD_SET(fd, &fdvar);
                max_fd = fd;
        }

        if (max_fd == -1) {
                fprintf(stderr, "wps ui utility: no fd set!\n");
                return -1;
        }

        n = select(max_fd + 1, &fdvar, NULL, NULL, &timeout);

        if (n < 0) {
                return -1;
        }

        if (n > 0) {
                if (fd >= 0) {
                        if (FD_ISSET(fd, &fdvar)) {
                                recvBytes = recvfrom(fd, databuf, datalen,
                                        0, (struct sockaddr *)&addr, &size);

                                if (recvBytes == -1) {
                                        fprintf(stderr,
                                        "wps ui utility:recv failed, recvBytes = %d\n", recvBytes);
                                        return -1;
                                }

                                return recvBytes;
                        }

                        return 0;
                }
        }

        return -1;
}

int
parse_wps_env(char *buf)
{
        char *argv[32] = {0};
        char *item, *p, *next;
        char *name, *value;
        int i;
        int unit, subunit;
        char nvifname[IFNAMSIZ];

        /* Seperate buf into argv[] */
        for (i = 0, item = buf, p = item;
                item && item[0];
                item = next, p = 0, i++) {
                /* Get next token */
                strtok_r(p, " ", &next);
                argv[i] = item;
        }

        /* Parse message */
        wps_config_command = 0;
        wps_action = 0;

        for (i = 0; argv[i]; i++) {
                value = argv[i];
                name = strsep(&value, "=");
                if (name) {
                        if (!strcmp(name, "wps_config_command"))
                                wps_config_command = atoi(value);
                        else if (!strcmp(name, "wps_action"))
                                wps_action = atoi(value);
                        else if (!strcmp(name, "wps_uuid"))
                                strcpy(wps_uuid ,value);
                        else if (!strcmp(name, "wps_ifname")) {
                                if (strlen(value) <=0)
                                        continue;
                                if (osifname_to_nvifname(value, nvifname, sizeof(nvifname)) != 0)
                                        continue;

                                if (get_ifname_unit(nvifname, &unit, &subunit) == -1)
                                        continue;
                                if (unit < 0)
                                        unit = 0;
                                if (subunit < 0)
                                        subunit = 0;

                                if (subunit)
                                        sprintf(wps_unit, "%d.%d", unit, subunit);
                                else
                                        sprintf(wps_unit, "%d", unit);
                        }
                }
        }

        return 0;
}

static int
get_wps_env()
{
        int fd = -1;
        char databuf[256];
        int datalen = sizeof(databuf);
        if ((fd = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
                fprintf(stderr, "wps ui utility: failed to open loopback socket\n");
                return -1;
        }

        write_to_wps(fd, "GET");

        /* Receive response */
        if (read_from_wps(fd, databuf, datalen) > 0)
                parse_wps_env(databuf);
        else
                /* Show error message ? */

        close(fd);
        return 0;
}


static int
set_wps_env(char *uibuf)
{
        int wps_fd = -1;
        struct sockaddr_in to;
        int sentBytes = 0;
        uint32 uilen = strlen(uibuf);

        if ((wps_fd = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
//		printf("dbg: socket error in set_wps_env\n");

                goto exit;
        }

        /* send to WPS */
        to.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
        to.sin_family = AF_INET;
        to.sin_port = htons(WPS_UI_PORT);

        sentBytes = sendto(wps_fd, uibuf, uilen, 0, (struct sockaddr *) &to,
                sizeof(struct sockaddr_in));

        if (sentBytes != uilen) {
		printf("dbg: uilen error\n");
		
                goto exit;
        }

        /* Sleep 100 ms to make sure
	   WPS have received socket */

//	printf("dbg: send success, wait for response\n");
        USLEEP(100*1000);
        return 0;
exit:
        if (wps_fd > 0)
                close(wps_fd);

        /* Show error message ?  */
        return -1;
}
