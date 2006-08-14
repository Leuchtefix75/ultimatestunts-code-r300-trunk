/***************************************************************************
                          usmisc.cpp  -  Misc utility functions
                             -------------------
    begin                : wo feb 2 2005
    copyright            : (C) 2005 by CJP
    email                : cornware-cjp@users.sourceforge.net
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <cstdio>
#include <cstdlib>

#include "config.h"

//internationalisation:
#include <locale.h>
#include <libintl.h>
#define _(String) gettext (String)

#include "usmisc.h"
#include "usmacros.h"

#include "cstring.h"
#include "lconfig.h"
#include "filecontrol.h"
#include "cfile.h"


bool copyConfiguration(CString &conffile)
{
	printf("File %s not found. Searching on alternative locations...\n", conffile.c_str());

	CString prefixdir = PREFIXDIR;
	printf("prefix = %s\n", prefixdir.c_str());

	std::vector<CString> locations;
	locations.push_back(prefixdir + "/etc/ultimatestunts.conf");
	locations.push_back("/etc/ultimatestunts.conf");
	locations.push_back("/usr/local/etc/ultimatestunts.conf");
	locations.push_back("./ultimatestunts.conf");

	CString sourceConffile;
	for(unsigned int i=0; i < locations.size(); i++)
	{
		CString &loc = locations[i];

		printf("Trying %s...", loc.c_str());
		if(fileExists(loc))
		{
			printf("found\n");
			sourceConffile = loc;
			break;
		}
		else
		{
			printf("not found\n");
		}
	}

	if(sourceConffile == "")
	{
		printf("WARNING: no configuration file found!\n");
		return false;
	}

	printf("Copying %s to %s\n", sourceConffile.c_str(), conffile.c_str());
	if(!copyFile(sourceConffile, conffile))
	{
		printf("  copying FAILED! (maybe we don't have the right permissions)\n");
		return false;
	}

	return true;
}

void shared_main(int argc, char *argv[])
{
	//the default directory of the configuration file
	CString confdir;
#ifdef UNIX_TREE
	CString homedir = getenv("HOME");
	confdir = homedir + "/.ultimatestunts/";
#else
	confdir = "./";
#endif

	//exceptions for the development environment:
	bool inDevelopment = fileExists("./execselect.sh");

	if(inDevelopment)
	{
		printf(
			"execselect.sh detected:\n"
			"  We are probably in the Ultimate Stunts SOURCE TREE\n"
			"EXECUTED FROM THE SOURCE TREE:\n"
			"  Using the conf file in the source tree\n"
			);
		confdir = "./";
	}

	//make sure that the directory exists
	makeDir(confdir);

	//make sure that the conf file exists
	CString conffile = confdir + "ultimatestunts.conf";
	if(!fileExists(conffile))
		copyConfiguration(conffile);
	
	printf("Using configuration file %s\n", conffile.c_str());

	theMainConfig = new CLConfig(argc, argv);
	if(!theMainConfig->setFilename(conffile))
	{
		printf("Error: could not read configuration file\n");
		//TODO: create a default one
	}

	//The data dir and the save dir
	CString DataDir, SaveDir;

	CString cnf = theMainConfig->getValue("files", "datadir");
	if(cnf != "")
	{
		if(cnf[cnf.length()-1] != '/') cnf += '/';
		DataDir = cnf;
	}

	cnf = theMainConfig->getValue("files", "savedir");
	if(cnf != "")
	{
		if(cnf[cnf.length()-1] != '/') cnf += '/';
		SaveDir = cnf;
	}

#ifdef UNIX_TREE
	//fill in home dir for "~/"
	if(DataDir.mid(0, 2) == "~/")
		DataDir = homedir + DataDir.mid(1);
	if(SaveDir.mid(0, 2) == "~/")
		SaveDir = homedir + SaveDir.mid(1);
#endif

	if(inDevelopment)
	{
		printf("EXECUTED FROM THE SOURCE TREE:\n  Using the data dirs in the source tree\n");
		DataDir = "./data/";
		SaveDir = "./saveddata/";
	}

	//find the absolute path
	//very long paths might cause segfaults
	CString absdir;
	char datadirbuffer[4096];
	if(realpath(DataDir.c_str(), datadirbuffer) != NULL)
	{
		absdir = CString(datadirbuffer) + "/";
	}
	printf("DataDir is \"%s\"\n", DataDir.c_str());
	printf("SaveDir is \"%s\"\n", SaveDir.c_str());
	CFileControl *fctl = new CFileControl; //TODO: find a way to delete this object
	fctl->setDataDir(DataDir);
	fctl->setSaveDir(SaveDir);

	printf("Enabling localisation\n");

	//select a language
	CString conf_lang = theMainConfig->getValue("misc", "language");
	if(conf_lang == "system")
	{
		char *retval = setlocale(LC_MESSAGES, "");
		if(retval == NULL)
		{
			printf("Setting the LC_MESSAGES locale to \"\" failed\n");
		}
		else
		{
			printf("Locale LC_MESSAGES is set to \"%s\"\n", retval);
		}
	}
	else
	{
		char *retval = setlocale(LC_MESSAGES, conf_lang.c_str());
		if(retval == NULL)
		{
			printf("Setting the LC_MESSAGES locale to \"%s\" failed\n", conf_lang.c_str());
		}
		else
		{
			printf("Locale LC_MESSAGES is set to \"%s\"\n", retval);
		}

#ifdef __CYGWIN__
		//F* Cygwin doesn't set the locale correctly
		setenv("LC_MESSAGES", conf_lang.c_str(), 1);
#endif
		setenv("LANGUAGE", conf_lang.c_str(), 1);
		setenv("LC_MESSAGES", conf_lang.c_str(), 1);
	}

	//select the Ultimate Stunts domain
	//We can only use ISO 8859-1 because of the font texture
	printf("  Package %s, directory %s\n", PACKAGE, (absdir + "lang").c_str());
	printf("  bindtextdomain returns %s\n",
	    bindtextdomain(PACKAGE, (absdir + "lang").c_str())
	    );
	printf("  bind_textdomain_codeset returns %s\n",
	    bind_textdomain_codeset(PACKAGE, "ISO-8859-1")
	    );
	printf("  textdomain returns %s\n",
	    textdomain(PACKAGE)
	    );
}

vector<CString> getCredits()
{
	vector<CString> ret;

	ret.push_back(PACKAGE " " VERSION);
	ret.push_back("");
	ret.push_back(_("Programmers:"));
	ret.push_back("  Corn� Plooy");
	ret.push_back("  Martin Boese");
	ret.push_back("");
	ret.push_back(_("Art designers:"));
	ret.push_back("  Borbola Balazs");
	ret.push_back("  Corn� Plooy");
	ret.push_back("");
	ret.push_back(_("Music:"));
	ret.push_back("  Pronobozo");
	ret.push_back("  Yllusion");
	ret.push_back("");
	ret.push_back(_("Translations:"));
	ret.push_back("  Borbola Balazs");
	ret.push_back("  Vincent Knecht");
	ret.push_back("  Michael Kronsteiner");
	ret.push_back("  Marcelo De Nardin");
	ret.push_back("  Corn� Plooy"); 
	ret.push_back("");
	ret.push_back(_("Website:"));
	ret.push_back(_("  Please visit www.ultimatestunts.nl"));
	ret.push_back("");
	ret.push_back(_("License:"));
	ret.push_back("This program is free software; you can redistribute it and/or modify");
	ret.push_back("it under the terms of the GNU General Public License as published by");
	ret.push_back("the Free Software Foundation; either version 2 of the License, or");
	ret.push_back("(at your option) any later version.");
	ret.push_back("");
	ret.push_back("");
	ret.push_back("		    GNU GENERAL PUBLIC LICENSE");
	ret.push_back("		       Version 2, June 1991");
	ret.push_back("");
	ret.push_back(" Copyright (C) 1989, 1991 Free Software Foundation, Inc.");
	ret.push_back("                          675 Mass Ave, Cambridge, MA 02139, USA");
	ret.push_back(" Everyone is permitted to copy and distribute verbatim copies");
	ret.push_back(" of this license document, but changing it is not allowed.");
	ret.push_back("");
	ret.push_back("			    Preamble");
	ret.push_back("");
	ret.push_back("  The licenses for most software are designed to take away your");
	ret.push_back("freedom to share and change it.  By contrast, the GNU General Public");
	ret.push_back("License is intended to guarantee your freedom to share and change free");
	ret.push_back("software--to make sure the software is free for all its users.  This");
	ret.push_back("General Public License applies to most of the Free Software");
	ret.push_back("Foundation's software and to any other program whose authors commit to");
	ret.push_back("using it.  (Some other Free Software Foundation software is covered by");
	ret.push_back("the GNU Library General Public License instead.)  You can apply it to");
	ret.push_back("your programs, too.");
	ret.push_back("");
	ret.push_back("  When we speak of free software, we are referring to freedom, not");
	ret.push_back("price.  Our General Public Licenses are designed to make sure that you");
	ret.push_back("have the freedom to distribute copies of free software (and charge for");
	ret.push_back("this service if you wish), that you receive source code or can get it");
	ret.push_back("if you want it, that you can change the software or use pieces of it");
	ret.push_back("in new free programs; and that you know you can do these things.");
	ret.push_back("");
	ret.push_back("  To protect your rights, we need to make restrictions that forbid");
	ret.push_back("anyone to deny you these rights or to ask you to surrender the rights.");
	ret.push_back("These restrictions translate to certain responsibilities for you if you");
	ret.push_back("distribute copies of the software, or if you modify it.");
	ret.push_back("");
	ret.push_back("  For example, if you distribute copies of such a program, whether");
	ret.push_back("gratis or for a fee, you must give the recipients all the rights that");
	ret.push_back("you have.  You must make sure that they, too, receive or can get the");
	ret.push_back("source code.  And you must show them these terms so they know their");
	ret.push_back("rights.");
	ret.push_back("");
	ret.push_back("  We protect your rights with two steps: (1) copyright the software, and");
	ret.push_back("(2) offer you this license which gives you legal permission to copy,");
	ret.push_back("distribute and/or modify the software.");
	ret.push_back("");
	ret.push_back("  Also, for each author's protection and ours, we want to make certain");
	ret.push_back("that everyone understands that there is no warranty for this free");
	ret.push_back("software.  If the software is modified by someone else and passed on, we");
	ret.push_back("want its recipients to know that what they have is not the original, so");
	ret.push_back("that any problems introduced by others will not reflect on the original");
	ret.push_back("authors' reputations.");
	ret.push_back("");
	ret.push_back("  Finally, any free program is threatened constantly by software");
	ret.push_back("patents.  We wish to avoid the danger that redistributors of a free");
	ret.push_back("program will individually obtain patent licenses, in effect making the");
	ret.push_back("program proprietary.  To prevent this, we have made it clear that any");
	ret.push_back("patent must be licensed for everyone's free use or not licensed at all.");
	ret.push_back("");
	ret.push_back("  The precise terms and conditions for copying, distribution and");
	ret.push_back("modification follow.");
	ret.push_back("");
	ret.push_back("		    GNU GENERAL PUBLIC LICENSE");
	ret.push_back("   TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION");
	ret.push_back("");
	ret.push_back("  0. This License applies to any program or other work which contains");
	ret.push_back("a notice placed by the copyright holder saying it may be distributed");
	ret.push_back("under the terms of this General Public License.  The \"Program\", below,");
	ret.push_back("refers to any such program or work, and a \"work based on the Program\"");
	ret.push_back("means either the Program or any derivative work under copyright law:");
	ret.push_back("that is to say, a work containing the Program or a portion of it,");
	ret.push_back("either verbatim or with modifications and/or translated into another");
	ret.push_back("language.  (Hereinafter, translation is included without limitation in");
	ret.push_back("the term \"modification\".)  Each licensee is addressed as \"you\".");
	ret.push_back("");
	ret.push_back("Activities other than copying, distribution and modification are not");
	ret.push_back("covered by this License; they are outside its scope.  The act of");
	ret.push_back("running the Program is not restricted, and the output from the Program");
	ret.push_back("is covered only if its contents constitute a work based on the");
	ret.push_back("Program (independent of having been made by running the Program).");
	ret.push_back("Whether that is true depends on what the Program does.");
	ret.push_back("");
	ret.push_back("  1. You may copy and distribute verbatim copies of the Program's");
	ret.push_back("source code as you receive it, in any medium, provided that you");
	ret.push_back("conspicuously and appropriately publish on each copy an appropriate");
	ret.push_back("copyright notice and disclaimer of warranty; keep intact all the");
	ret.push_back("notices that refer to this License and to the absence of any warranty;");
	ret.push_back("and give any other recipients of the Program a copy of this License");
	ret.push_back("along with the Program.");
	ret.push_back("");
	ret.push_back("You may charge a fee for the physical act of transferring a copy, and");
	ret.push_back("you may at your option offer warranty protection in exchange for a fee.");
	ret.push_back("");
	ret.push_back("  2. You may modify your copy or copies of the Program or any portion");
	ret.push_back("of it, thus forming a work based on the Program, and copy and");
	ret.push_back("distribute such modifications or work under the terms of Section 1");
	ret.push_back("above, provided that you also meet all of these conditions:");
	ret.push_back("");
	ret.push_back("    a) You must cause the modified files to carry prominent notices");
	ret.push_back("    stating that you changed the files and the date of any change.");
	ret.push_back("");
	ret.push_back("    b) You must cause any work that you distribute or publish, that in");
	ret.push_back("    whole or in part contains or is derived from the Program or any");
	ret.push_back("    part thereof, to be licensed as a whole at no charge to all third");
	ret.push_back("    parties under the terms of this License.");
	ret.push_back("");
	ret.push_back("    c) If the modified program normally reads commands interactively");
	ret.push_back("    when run, you must cause it, when started running for such");
	ret.push_back("    interactive use in the most ordinary way, to print or display an");
	ret.push_back("    announcement including an appropriate copyright notice and a");
	ret.push_back("    notice that there is no warranty (or else, saying that you provide");
	ret.push_back("    a warranty) and that users may redistribute the program under");
	ret.push_back("    these conditions, and telling the user how to view a copy of this");
	ret.push_back("    License.  (Exception: if the Program itself is interactive but");
	ret.push_back("    does not normally print such an announcement, your work based on");
	ret.push_back("    the Program is not required to print an announcement.)");
	ret.push_back("");
	ret.push_back("These requirements apply to the modified work as a whole.  If");
	ret.push_back("identifiable sections of that work are not derived from the Program,");
	ret.push_back("and can be reasonably considered independent and separate works in");
	ret.push_back("themselves, then this License, and its terms, do not apply to those");
	ret.push_back("sections when you distribute them as separate works.  But when you");
	ret.push_back("distribute the same sections as part of a whole which is a work based");
	ret.push_back("on the Program, the distribution of the whole must be on the terms of");
	ret.push_back("this License, whose permissions for other licensees extend to the");
	ret.push_back("entire whole, and thus to each and every part regardless of who wrote it.");
	ret.push_back("");
	ret.push_back("Thus, it is not the intent of this section to claim rights or contest");
	ret.push_back("your rights to work written entirely by you; rather, the intent is to");
	ret.push_back("exercise the right to control the distribution of derivative or");
	ret.push_back("collective works based on the Program.");
	ret.push_back("");
	ret.push_back("In addition, mere aggregation of another work not based on the Program");
	ret.push_back("with the Program (or with a work based on the Program) on a volume of");
	ret.push_back("a storage or distribution medium does not bring the other work under");
	ret.push_back("the scope of this License.");
	ret.push_back("");
	ret.push_back("  3. You may copy and distribute the Program (or a work based on it,");
	ret.push_back("under Section 2) in object code or executable form under the terms of");
	ret.push_back("Sections 1 and 2 above provided that you also do one of the following:");
	ret.push_back("");
	ret.push_back("    a) Accompany it with the complete corresponding machine-readable");
	ret.push_back("    source code, which must be distributed under the terms of Sections");
	ret.push_back("    1 and 2 above on a medium customarily used for software interchange; or,");
	ret.push_back("");
	ret.push_back("    b) Accompany it with a written offer, valid for at least three");
	ret.push_back("    years, to give any third party, for a charge no more than your");
	ret.push_back("    cost of physically performing source distribution, a complete");
	ret.push_back("    machine-readable copy of the corresponding source code, to be");
	ret.push_back("    distributed under the terms of Sections 1 and 2 above on a medium");
	ret.push_back("    customarily used for software interchange; or,");
	ret.push_back("");
	ret.push_back("    c) Accompany it with the information you received as to the offer");
	ret.push_back("    to distribute corresponding source code.  (This alternative is");
	ret.push_back("    allowed only for noncommercial distribution and only if you");
	ret.push_back("    received the program in object code or executable form with such");
	ret.push_back("    an offer, in accord with Subsection b above.)");
	ret.push_back("");
	ret.push_back("The source code for a work means the preferred form of the work for");
	ret.push_back("making modifications to it.  For an executable work, complete source");
	ret.push_back("code means all the source code for all modules it contains, plus any");
	ret.push_back("associated interface definition files, plus the scripts used to");
	ret.push_back("control compilation and installation of the executable.  However, as a");
	ret.push_back("special exception, the source code distributed need not include");
	ret.push_back("anything that is normally distributed (in either source or binary");
	ret.push_back("form) with the major components (compiler, kernel, and so on) of the");
	ret.push_back("operating system on which the executable runs, unless that component");
	ret.push_back("itself accompanies the executable.");
	ret.push_back("");
	ret.push_back("If distribution of executable or object code is made by offering");
	ret.push_back("access to copy from a designated place, then offering equivalent");
	ret.push_back("access to copy the source code from the same place counts as");
	ret.push_back("distribution of the source code, even though third parties are not");
	ret.push_back("compelled to copy the source along with the object code.");
	ret.push_back("");
	ret.push_back("  4. You may not copy, modify, sublicense, or distribute the Program");
	ret.push_back("except as expressly provided under this License.  Any attempt");
	ret.push_back("otherwise to copy, modify, sublicense or distribute the Program is");
	ret.push_back("void, and will automatically terminate your rights under this License.");
	ret.push_back("However, parties who have received copies, or rights, from you under");
	ret.push_back("this License will not have their licenses terminated so long as such");
	ret.push_back("parties remain in full compliance.");
	ret.push_back("");
	ret.push_back("  5. You are not required to accept this License, since you have not");
	ret.push_back("signed it.  However, nothing else grants you permission to modify or");
	ret.push_back("distribute the Program or its derivative works.  These actions are");
	ret.push_back("prohibited by law if you do not accept this License.  Therefore, by");
	ret.push_back("modifying or distributing the Program (or any work based on the");
	ret.push_back("Program), you indicate your acceptance of this License to do so, and");
	ret.push_back("all its terms and conditions for copying, distributing or modifying");
	ret.push_back("the Program or works based on it.");
	ret.push_back("");
	ret.push_back("  6. Each time you redistribute the Program (or any work based on the");
	ret.push_back("Program), the recipient automatically receives a license from the");
	ret.push_back("original licensor to copy, distribute or modify the Program subject to");
	ret.push_back("these terms and conditions.  You may not impose any further");
	ret.push_back("restrictions on the recipients' exercise of the rights granted herein.");
	ret.push_back("You are not responsible for enforcing compliance by third parties to");
	ret.push_back("this License.");
	ret.push_back("");
	ret.push_back("  7. If, as a consequence of a court judgment or allegation of patent");
	ret.push_back("infringement or for any other reason (not limited to patent issues),");
	ret.push_back("conditions are imposed on you (whether by court order, agreement or");
	ret.push_back("otherwise) that contradict the conditions of this License, they do not");
	ret.push_back("excuse you from the conditions of this License.  If you cannot");
	ret.push_back("distribute so as to satisfy simultaneously your obligations under this");
	ret.push_back("License and any other pertinent obligations, then as a consequence you");
	ret.push_back("may not distribute the Program at all.  For example, if a patent");
	ret.push_back("license would not permit royalty-free redistribution of the Program by");
	ret.push_back("all those who receive copies directly or indirectly through you, then");
	ret.push_back("the only way you could satisfy both it and this License would be to");
	ret.push_back("refrain entirely from distribution of the Program.");
	ret.push_back("");
	ret.push_back("If any portion of this section is held invalid or unenforceable under");
	ret.push_back("any particular circumstance, the balance of the section is intended to");
	ret.push_back("apply and the section as a whole is intended to apply in other");
	ret.push_back("circumstances.");
	ret.push_back("");
	ret.push_back("It is not the purpose of this section to induce you to infringe any");
	ret.push_back("patents or other property right claims or to contest validity of any");
	ret.push_back("such claims; this section has the sole purpose of protecting the");
	ret.push_back("integrity of the free software distribution system, which is");
	ret.push_back("implemented by public license practices.  Many people have made");
	ret.push_back("generous contributions to the wide range of software distributed");
	ret.push_back("through that system in reliance on consistent application of that");
	ret.push_back("system; it is up to the author/donor to decide if he or she is willing");
	ret.push_back("to distribute software through any other system and a licensee cannot");
	ret.push_back("impose that choice.");
	ret.push_back("");
	ret.push_back("This section is intended to make thoroughly clear what is believed to");
	ret.push_back("be a consequence of the rest of this License.");
	ret.push_back("");
	ret.push_back("  8. If the distribution and/or use of the Program is restricted in");
	ret.push_back("certain countries either by patents or by copyrighted interfaces, the");
	ret.push_back("original copyright holder who places the Program under this License");
	ret.push_back("may add an explicit geographical distribution limitation excluding");
	ret.push_back("those countries, so that distribution is permitted only in or among");
	ret.push_back("countries not thus excluded.  In such case, this License incorporates");
	ret.push_back("the limitation as if written in the body of this License.");
	ret.push_back("");
	ret.push_back("  9. The Free Software Foundation may publish revised and/or new versions");
	ret.push_back("of the General Public License from time to time.  Such new versions will");
	ret.push_back("be similar in spirit to the present version, but may differ in detail to");
	ret.push_back("address new problems or concerns.");
	ret.push_back("");
	ret.push_back("Each version is given a distinguishing version number.  If the Program");
	ret.push_back("specifies a version number of this License which applies to it and \"any");
	ret.push_back("later version\", you have the option of following the terms and conditions");
	ret.push_back("either of that version or of any later version published by the Free");
	ret.push_back("Software Foundation.  If the Program does not specify a version number of");
	ret.push_back("this License, you may choose any version ever published by the Free Software");
	ret.push_back("Foundation.");
	ret.push_back("");
	ret.push_back("  10. If you wish to incorporate parts of the Program into other free");
	ret.push_back("programs whose distribution conditions are different, write to the author");
	ret.push_back("to ask for permission.  For software which is copyrighted by the Free");
	ret.push_back("Software Foundation, write to the Free Software Foundation; we sometimes");
	ret.push_back("make exceptions for this.  Our decision will be guided by the two goals");
	ret.push_back("of preserving the free status of all derivatives of our free software and");
	ret.push_back("of promoting the sharing and reuse of software generally.");
	ret.push_back("");
	ret.push_back("			    NO WARRANTY");
	ret.push_back("");
	ret.push_back("  11. BECAUSE THE PROGRAM IS LICENSED FREE OF CHARGE, THERE IS NO WARRANTY");
	ret.push_back("FOR THE PROGRAM, TO THE EXTENT PERMITTED BY APPLICABLE LAW.  EXCEPT WHEN");
	ret.push_back("OTHERWISE STATED IN WRITING THE COPYRIGHT HOLDERS AND/OR OTHER PARTIES");
	ret.push_back("PROVIDE THE PROGRAM \"AS IS\" WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED");
	ret.push_back("OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF");
	ret.push_back("MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.  THE ENTIRE RISK AS");
	ret.push_back("TO THE QUALITY AND PERFORMANCE OF THE PROGRAM IS WITH YOU.  SHOULD THE");
	ret.push_back("PROGRAM PROVE DEFECTIVE, YOU ASSUME THE COST OF ALL NECESSARY SERVICING,");
	ret.push_back("REPAIR OR CORRECTION.");
	ret.push_back("");
	ret.push_back("  12. IN NO EVENT UNLESS REQUIRED BY APPLICABLE LAW OR AGREED TO IN WRITING");
	ret.push_back("WILL ANY COPYRIGHT HOLDER, OR ANY OTHER PARTY WHO MAY MODIFY AND/OR");
	ret.push_back("REDISTRIBUTE THE PROGRAM AS PERMITTED ABOVE, BE LIABLE TO YOU FOR DAMAGES,");
	ret.push_back("INCLUDING ANY GENERAL, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES ARISING");
	ret.push_back("OUT OF THE USE OR INABILITY TO USE THE PROGRAM (INCLUDING BUT NOT LIMITED");
	ret.push_back("TO LOSS OF DATA OR DATA BEING RENDERED INACCURATE OR LOSSES SUSTAINED BY");
	ret.push_back("YOU OR THIRD PARTIES OR A FAILURE OF THE PROGRAM TO OPERATE WITH ANY OTHER");
	ret.push_back("PROGRAMS), EVEN IF SUCH HOLDER OR OTHER PARTY HAS BEEN ADVISED OF THE");
	ret.push_back("POSSIBILITY OF SUCH DAMAGES.");
	ret.push_back("");
	ret.push_back("		     END OF TERMS AND CONDITIONS");
	
	return ret;
}
