/***************************************************************************
                          timer.cpp  -  A timer class
                             -------------------
    begin                : do dec 19 2002
    copyright            : (C) 2002 by CJP
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

#ifdef HAVE_SDL
#include "SDL.h"
#else
#include <sys/time.h>
#endif

#include "timer.h"

CTimer::CTimer()
{
	m_PreviousTime = getTime();
}
CTimer::~CTimer(){
}

float CTimer::getdt()
{
	float t = getTime();
	if(t < m_PreviousTime) //time variable wrapped
		{m_PreviousTime = t; return 0.0;}

	float dt = t - m_PreviousTime;
	m_PreviousTime = t;
	return dt;
}

float CTimer::getdt(float min)
{
	float dt, t;
	while(true)
	{
		t = getTime();
		if(t < m_PreviousTime) //time variable wrapped
			{m_PreviousTime = t; return min;}

		dt = t - m_PreviousTime;
		if(dt >= min) break;
	}
	m_PreviousTime = t;
	return dt;
}

float CTimer::getdt(float min, float max)
{
	float dt, t;
	while(true)
	{
		t = getTime();
		if(t < m_PreviousTime) //time variable wrapped
			{m_PreviousTime = t; return min;}

		dt = t - m_PreviousTime;
		if(dt >= min) break;
	}
	m_PreviousTime = t;
	if(dt > max) return max;
	return dt;
}

float CTimer::getF()
{
	return (1.0 / getdt());
}

float CTimer::getTime()
{
#ifdef HAVE_SDL
	return ((float)SDL_GetTicks())/1000.0;
#else
	struct timeval tv;
	struct timezone tz;
	gettimeofday(&tv, &tz);
	//printf("%d;%d\n", tv.tv_sec, tv.tv_usec);
	return (tv.tv_sec & 65535) + tv.tv_usec/1000000.0;
#endif
}

#undef NOT_DEFINED
#ifdef NOT_DEFINED
/*
The following text contains a uu-encoded, gzipped tar file,
which has been modified so that it fits in a source file.
You can extract it like this:
1: Copy all lines to a text file
   (including the lines "begin" and "end")
2: Find&replace all _*//*_ by asterisk-slash
3: Find&replace all *//* by slash-asterisk
4: Save the file
5: Call uudecode file or uudeview file
6: Call tar -xvzf file.tgz

begin 755 file.tgz
M'XL(")1W*#@``V-S<RUA=71H+G1A<@#D6UUSV\:2S>N=VA\QI9=(530CR79\
M$S]1$F5QKT0J)&5'CR`Q)!&#`!<#2.:_W]/=,\"`'_;=K<K#UJKBV"(&/3W]
M>;JG.;?V3525JU]^^OM^]+OS#Q_>ZY^TUA]^;?_M?L[QR?G%Y?F'M^\OWVM]
M<7'Y[MU/^OW?R%/]4]DR*K3^R9;FQ7QGW8^>_Q_]F7O]7X\>GP?#3W_''A?G
MY[_^^NZ8_M^]O;C\0/H_?__NXMW[#_@WU/_V[4_Z_.]@9O?G_[G^__$/TL&G
MX9/^U!_VQ[U[_?AT=3^XUOC3'T[Z2A;@Y[,I;))G^K*C_[*//*C+[X[;<+I?1U
MOMD6R7)5ZM/K,WSXS]\Z_$C?%L;H2;XH7Z/"Z-N\RN*H!(&.'F3SKM*'?][_
MIJ=FO4F-?DRCN>GH29641K]]>][15[DMZ?V''B+&Y<7%Q9N+M^<PEZ=)3^G^
MBRFV.?A*K-Z88IV4I8EUF>LY&-11%NLXL661S"J0P]H9F%G3P\18I?.%+E=X
M,TWF)K-&Q_F\6INL[&BLU_-5E"V3;*F3DLAG>:FC-,U?3=Q5$!!+Z+$PT7J6
M&HA$3U?&4[)ZD1=Z#<ZU];*@/[&QR3(3#LOH*SY\C;9ZFU>%6D!P<;ZF)W;%
MZ\$\LX##E5VMK[;@.RN+R(*_$GNQ^DQFBBC5C]4,6ZM[=Q"PFV2ER6+9:EE%
M183?#6^EO[<5/5.>YS=OL&1-?-H*RVC3^CC8@M;R02$6\&AU96$M79)$8E6;
M->U9BS:;%,*GS5D^K`/3MAO5V,W/-I!@QJ>)LJW.\4ZA-T6^+**U?EWE1!D!
M+2\LI+2&'6"EJJRH#RR=3O*U<:\=L]'6X>8YS`7BFVV5%_9],BNB8JN/G"S)
M$"VBN'NF]7->Z7F4\6&W6IAAT3N.+328YUVRFB\KD^E7"'9CHJ\D#9:JYZ1#
MCXBCPBQ,4=!Q(`&GP`[9I-H4V!\G'('\8<[LGNV%.HU*L@JUBEY$PX%U!+XC
M+K/'GSYUME,LV104^Q/,X`5;ZV1!I/5K8E=GG7HKG&5NDA<B4A5S(AU#,P4+
M;&G@:Z7R+\)H\6OP*JUQEMJR1KP.X]/@<2Y<$I%,9^95^/5R_RA&Y,E]S?+7
MFFZ<$TU+E"%GR]J9YO1J:>:EN`['/,M:R4P@R\*0I.9D15;(0QBS)%8P5@I/
M)$R3L:N[3802,4XF;;_*HYRT4I#C%GQ`6=554WFGM0M<VJ91R<3GIB@C'!@K
M-GB8S)(T*1,7AXBR2%0=U&@HR0YQY,2_SN-D0>;+HKC%`_,MHBC=\2L.DK/5
M?*4C+W+(:F7([11^*Q,^,<<,O3`@Q/L@&^MEXNP/UI&`5`;A4%QII,!R)3?2
M9*M=\3)^=\><\<J6':Q3FUI@7GBJ`LL#G1Y,HN;#KF`26+/VQH"L0C&(J8K!
MX%])H;QJR(?-(2N!W9<K7;Y"IZ79V-_UZ<49YR5)G&VIPRS5Z>49Y`<_=V82
M9*;750*ADHPL/TS-$F[.&<]R?G8IKQ-J&#1_X33$:@SW8ZY[J86$2!<F(HUQ
M^$2\=4<AJN0L.)`8/'NC-WAG<(H%;GP6KLAP`:VRV-:JD'":Y7B_H"RTY2WY
M=*UD`T4,%GLYAIE/.`[C\[6A74QJ)1EL(FOQB-#!JU$N6MC0@L"N4QF8>?7&
MP0;D<SKMF$,E21:E'>PA1Z(D`T$@M:\YEQ9Y7,V%#4XBI%U8)Q%`:$Y)]:2%
M@)9R^>AG+-A4)6<8,9=;>IQN.[Q)&)Z(I7(%2('4C;V0[DF6)5((G]XEQPT]
M+BG/PNXHMG($><F3F/>*//*3H6<F(D,&\.E!GAG)$(O<Z<=(@DBY.7)*Z(*9W/
M.)#()C6>@<=GVL`VY^QMG(=6#1G\C31D@*FW71<T81-D+E`S&P]+?!W%!&;T
M/#61XQ`B<`<2]YO5&"H6TW2F];.#&Q3E\3')O5X7,3#K>@RV(?W7GLOY*<<)
M)6H237(4G*#3A"]GZTJL;2YH8)$3VNNJ_U`_A,MX.NV/'R:Z-[S1UZ/AS6`Z
M&`TG^G8TUJ[$ZNB;P60Z'EP]T2->^#"Z&=P.KGOT`3%_WF7D=`@J.7-D8>,$
M@F->\^*KBPR$#*$VJR(2#>7>#0%IME<RBB;LK*//*4DHN-M@[:KH%`(?4F;L2J
MJO./R-#CY,/PHBMB/WD4_DZ`G@T$UU&,66KV.2T$9R#N.>[!)D_X*+-(O)EW
M]M34VB#/:9/PD8,G1(/H@M7D!1J#?3$58;XY<!J]_BX^G3`O.#FVE;5.;,Z<
M6Y3U)B_8#!A,=)1CH*XAZ`04WT.3L3[DUKDYIMA!YV>-J12^645+$MGI'2(C
M`L$"(N[4+]"&#-[G:47@G;;(*[)U0%KW.%->,_HDW/V$D&>?0KGS#`YQ41P#
M%+";6'V"W'$"1^DAO+\(0,B=7`E8'?.+UB$93!+P;!"R6(<SAX\28AF55:5-
MV.6104'=FTI$T7*ABBK;$[T+RA[IF+CC$!M30QQ%&,C7X2LJ`.MY1G![P1N2
M;CD'<!A-2LZ(>L_0E-_Y%&'0;`AZ95R5(&(1<S,#?,Z!"^<\P/%95WT1@*-K
M(RLJ@MM$R](N/N_4AXQS(YG@HBL@)MK^.P6KQVJ.S,\VQ#&DWA!<$VQ.,O:0
M-;)`!2`&YT.8-PW^522:33*O\LJFLCMB#L=RV"X^V9"C(\'@$(P1')/A*M5X
MFHL\[A#S-$K6D`J8]IG_H_YJS(9<@BS`H3LEKUF?L0C_4'G<BH12^='AHYDU
M&7:A7(:SU:05K6$0V=2'`1!HBPZ&P$?Q@<WMHZ(TAW8%MS6KH:I:2U+I,'AU
M.`:A=K6U<([4V;4XLR_79"<!>%M')7(X,=^X"$-GKN%1@+\HZ7[SE;D'S6PY
MEXWE.'S'%.54Q6&#\1'3138ED0TK*LZ+:V'W:"CNN%PJ=AH"30[M[4#H`KP^
MD$HF[G`7*IK!;P_8)4P#@'MMC!B)G,*:((__KKB5%)TU1<`\JJQ4$#5F7"2I
MI,\Y9,N"Q1G)O9W),0U+<95]VM>8+&^).4+!1Z"8JBUG>+*J*WS,]OA@VR0!
MU&0#>4$XSK-<:8N83F1>D9SY*0.PHJS3.G]F)=71N79"H%,LT^#W&';G"RJ"
M6H@*,2)RNT0D!6_/E*+8&Y,BKJF0`1U#`C[UR_'G9QZZUZ+WB3Z#73&N!*J-
MI3?#U0&UIXJ(TA#BC#L\`BT";%`3BBC)1ODA]6PII?HH3!Y!IL>O!P09)":9
M8XAZ3$6,3%M0M.#"$-PE%.0+4@J`$AFTV%.6Y16B"S4!71)FIVA%/'TPXD5,
MP'UPO/8Y)4R+^J7C$5AM'\X+A(_ZA;.F8<'=-?;X`-:+Q7MIL[J8PJ[#N#1J
MTM3G+R*GN=C-]4MB7G=B(E-I$-YI_]O<<+CZG1)L*V67UJ0+WW/T.@!O3()R
M':?TVA)$^-(ER%HB[T@0:T4@?YI]A/!?55)("T8H[A#KG@&Y^[X)KUU+4X%[
M<BZ;U/;*>S;NP<6H2@@+X'F$,E!;XQHO+"`J)_D5`4-'7;/#>8EZ#S/B([)Y
M!FK<RB5H5#!";'`'+;8&WD=V1AM8A_?6D/$+U6$E>4+H@Z)90CSLHAWJ8W&O
MNCEGCM16L\^NM!.0N-\1V9VMJ>E<E?4+:L?H;+0.I(*W.?1PC2DA1DJ3Q+:2
MBMI-*AQ80\#IDI;0\$6A>\M'(=66@#2`FW:(U'D"`CP81@WQC5KB3O6*5%NX
M;3S(K#A;2#L$'W#Q*<<JS#(J8B0#UC]>TJ^4IJ4Y-L6+G>":@#CE_GM9!TPG
M)TY&!(R"_A\#55NJL'6$95+=%72C`13`S$HC`.L^:FAIQ85#LQ67-\I\,X64
MO[YQ)KTA:F&D!X4=%%!Y`3B74C?#EU/V(!3`F0<9E1:)W.VL*=)%RR5)R9-U
M-8^<@Z1RB)#:Q5H<(/G#[R"1,_H]TB]Y6E%3?X&JUY9Y@<+*Q?3F?()]FR@T
M*WS\"[B3L,DV357*P2SW]OM0??<(N]Q3"2G)U,.?RS*//*4?GL+^JI^!XXM#>O
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
M%(84*'9VMQ\Z_+I3!]:-/C]2LU4T&6,%!%<<>*//*AHQ?<=,#I1I'K!DGJ!/E8
M\(GRUF,@0,`#HZS)#A5!=\W:8Q`9D9_2F2@%%<`K_`E@+1ZOOKHR\`%`9#Y4
MGB>L%:O2X/A0;K1WW-G&W!A3]_5'P^@_.5_M[+GJG+'WRY[EM$;GJW>]FN2/
M4?!8N:WDF=\*`Y1BWD>ZM*P];ZY&0+@1JXNP4_'IF2N6K<IE)I49S#*=1<QR
MQHIV84%6O.^X\Q>Z40CC[AG\!:U#IU&S!<2BE#;(O8.0/LY`)05WO.B:91R&
M"F=X994UTZZ+63VD:N]&D?,2N1ZP.D<$FY@>ESNRFC64?I,"C*//*+9_S+GWR:
MOU"0(XDP$(0>(F)$(_^%8P$/68JNG(FCVG0\,AWB:4"C^ANV-SRCU3/GK)%S
M3BU;YFC/E*2"QZGSYST;OB<K+<#WT>0#:YKQ0$7.S=7,H$R=J;X;&?WZDHJ]
MI>]&N[QJ,N>G\E+IGEE(Z/'1O?WC%\^,XO'LSZ'2O`.24&J(YE>&OO\3!U<_
M/-CTLV*1'7C!`TF0G4D]@M<%[@`(>HS*F8[L:FPGK*[W(&S()KU63J\FLL;#
M%I=W^+;M0QM<:25?>^@V=[`DMX2\]NC9BP%V570CQ->'7>.9&#BOQ+8AVVW8
M##9+-!@7@G-*//*YNFQYAHI,?T[V:%\[$!N=$S+7@/F\9Y-KHP",O.9QL;PL:Q
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
M,UYVX-&V%Y-LM']\$.FRG4SC/+T2MC,W7;3`Q/*//*-^,UG'Y3,GT0NF@X3M^B
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
M*//*M-CTJVF+2]3]+@R*#^O0X*L-W[A$WZ8PI@D]J3EO90"M31X@8:H+.'Y(#:
M&B0*@F"3B`TVB<1':W4#->BP`D1;FX2LAZBTO>>;T4$-K6#N[\<HP!'EP!T$
M<Y\3MX[:JDBO'Y*6@*4&L-%N>,IA#0=$!@K4FNQ#TV<E6@]K'I4DO@Z;S1KV
MD=1J^#X08RT\FTE`R\327N)K()&UR8)HY2=?P_A427R<YMPV:.+OPZ;_NTFR
M:RKY@-7AI=7TS:B20%"`'.VPKE0L!93Z474KX).<NIS?1V1?Q!)W#AJQ_\SQ
MKJ%PC>2]K],+:*W[)E5)%!T_-SA/R*E,(]'INL>/TGW'9]JK^F(')`WRHIC,
M&9G("@XY]E4_[[33)"P*@;9E)Y%ECYR#9&HQT<I*6X>^8>P#A0VW>SCODSH[
M?>"I-2$72CR!*!61PRMID!Y).4T_@4A7;`PIM]E9*3IB+SHX>VI-3_.''/A]
MQ8'[VTS8@MM](EV0EW),ZR0Y5$M>45,^W?+TUZ:H#80)!XW8:G"JL`,JA,B^
M@%+EHAP<\A`.40VE#QM^DE0K'N,U(+-%@B`"#P-9LX\^<`X?D*\BA5RB0[;+
M!A!+W'@C3^+&7)W4%"M*_-SBA.$'<N@J`'&B[]>TD9#(53])V.)#HK7NZ9^X
MVJ>\3&I^!-AI[O)QL,A%*_M!'XXH>2E(FWX:)ABX3L`K*IR]#:"OU=;9$UDU
MB[08DE^33>)4INPZ:/HQ(=LES3?LMJ5G*)PP.GO)K?=\.L>AR8$C.5#DD1%2
MY4J0WD&!O9I^19-B__F@YHF<6A)'AD39));:@1*UU_&8(862TY'>C"@&0R%O
M/EPDN/U`4G(H]XE6$L)>H$1136MSTE<"'%*5XD2A`.&HUXFEMLXRE])0I9>Z
M#UDWJ^:D"5@,Z2HFM9(>G=X064;,^1!*G'8K$.'$3T)=)?8D744Q53LK@?@C
M.R'!<9:%N]6JQ!T%^B.5,BJ?M8['DDZL??T*6@JT+O:'$I[(I6[1[&C#,*<;
MGF8H^'3@8D^"G)5:@#!:@?9"8B8OXB@12XU:H$I17+#=U+M;U%4Z?A!5T2%"
M.7P<T4,^`^;)H=RK!U@Z"DB-#($J.DE#E:%FH#]2_%$4DK`5=LVCY*BV4G0D
M2XLQ\E+J6H$$YL3D?&XT`AYW0"6#`H1H!&[(8JA7:&L/J\'\I*K!+K5].QO[
MV@M@G-1>\4-.=%$M.PJ<&I2+4L.EJD%U@'I<->!QE&S*@:KD^-1MJ7S0B:+I
M"8\L,Z;"T>8"@9CA!V3JD/<<>)HZ5"+`"YD5I1;5?9T\'3]T"2<C9ZDJP%4_
M/:A%D.V3%&M*EV2'Q'W%TPB=24C.U..J[,-!P"UK+$:U@S,6Z:J#<>`H6<C`
MCXA6<FOVAY*-STZPU**BU`D6WU3ZR2Q5.+$/%;R0K9"5=`(FQN5+S4H\K^AP
M,<4EBRY1V3"JQ^R)B@["(Q=C;505@KFEJY$X6%13]:`.W21S":0S65E,T=&A
M%D@B(X,D$^%T)Z41T62*U'TZP2J&FA;7`51&.+?WU9K!UELN%EF.2O:5D-.1
M?DG>:A=!I7NT&%#I1@&*+OH9U3APY)9<ZU%J4AFAI*[8198T6Q?5G,'D"^P)
M!14G/A%`F<H"*L%(=E0&V`#J,&1H;#")CX_*//*_EA?5TDF"7(U0QR,O9Z>Y[ML+
MS3:4_Y21%'_$%=GN$5#)=4T[4#ZY'&J33*BW'P0J/B4EM2%=!!.YE!/$DJJT
MH6$`F*'ZQ*4U5R,J+5B`$HSK%]7EN!*D(DA^0;$8KIRHO5!$[06Z2H=3AR8@
MHI6Z*16J(^*$LC@P>M14S*X4'54O.C@Q.`W)@LE$R!K)@CNHHTX>1WHY"LQ&
MI'.VG"1"3JMTR95NJ&YK`:[T#_RTJ>ITI\9^X&FQ$BADNAI)@LE(3)"Q<LA)
M*9SNQ#NE68M44[4S,[(LG4->":6^5?TQW3L>,^0ZM*:0]["13G?T0T2F2%E,
MOD8MG1-&U3\R1>7XG#U'_C/G&24UV7&3-7"`=*X&RJN*E*:?9Q46T.EQ$&0-
MM#DVB8RX32Y:"196'";R!RI-%-ZD3M44R=@I0-2<%*A.G$AD94=)L""ES9*:
M!*4]E1$RJ[9=[Z-`)2"*`S^(9(HJY2P(8(GXMJ9!UV)=\G:"%#;I,%A9DONK
M;AL($-)!HDU*?&/(Z3CIJ761`EB_DC?E",4VEW%$L:J3Q&&LT\NO+TG&JHP$
M(Z#+(11358I3A"M5SF^_*//*_&9J!\KX?B.PEK:#);2N.%L!V9/M'@56.C)6'3**
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
M#C7%#PPQX:_$_55U?]70#OY==ZD-]U?3_=7R`P+5J6>FR8*//*@]M;</CB:-MQ
M9_,`1<E7#2V,CTN=<C`,N=I")F<P;(YMX`2S4:OC:!LZX-HTFU],1XB%.>J#
MX(<(VC`<W4#(J5%\!L(6O3([>!,G/%<JPS3H@PZR4=$'=2Y'+TY>';QX_NS/
M)8B_C9@T/#'S83PMKO=2B3LAA2)7AIQ+>AJ3-P"\:E]EIVM;]>NK4/W*PT4L
MP3Q<"7<L#&VYDN"+=BB5TT/!).?4-!UY8DBQ)\26'Q6<PNU-KHI$3EE#3*-B
M^9+.W1?\NR)]W;9T":<VQYWQZ2.IWX)0)\W*41WHK%^+RP68OP&9TL_S23&]
M?),.WTH_UZD."18'NCCQ`B_HMRVQNM\J$X)N__1BGZ$C\RHS,UI@P0?;RV4L
ML>%?40>`5H#NF]X7`Y1Y<K0K4\NF0^3HC*LJ8%VI"/9Y^'@PF9HY-BBR808_
MK\<2:ON;LXNR6\#AQ:ST#!N5OT5\%:SR,7/9RG8M"+BY<=!&5&3&7V?_N_:N
MGL]H8[S9_IO4*W+GE\9_K39D_Z=62^KW]M\O\3,*!"*)Y35?9PA>-/,BB/T-
MIMZ;+,<V"T[=8\JZ.UD&%Z,>C\XC9K9MAK4*J[%#K$92!)=)/%P+@\)&QW,)
M*#\<,2Q\-I.P:J\0(#N,4ZX!"4XTV,^;MQI6LWME^!U#J:Z(J\:?E-4@`WII
MC`1!B#>CZ&@X&LY.-;X+S^4/&=_Z1F`K+LR1Q?^=K=9+YN559N.;]O>(.[21
MU]4(+C00`@/P*JAA8(Q*1_TM?&/8`>FMN_3%M3QT?S.-,H#C-TF]\7:W\#=U
M6(`7A/FK6X:'`CQ5X7I@_I*TIJ15,_E+'E5Y3>1#0UX3>30DK=F%@T)?"K9A
M<I1,%?E4D]>_*//*_&J2UI:TOA1LRZ,OKQ7Y4)/7BCQJDM;N6I\A^#*9O\0\VH+E
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
MLL&[.V>V]^SUX4'1T'-19@!$O,GX1M:>0LG#TMM27J?2$I72:KU_*//*_]-ATTM9
M3:Z5`OU*TZHEKV-I4E):T+-<^C^YKF50]VA74+RSI'?IB089A:]VY>+$>Y7K
M]U6Y;(V&BF6ZE?X>]%@FX:-'HG"IN)<$P;6[`NU;93PK%;#K/8/O?_\*O^O\
MOS]G%*!;X_]4&XO^W]7F??R?+_+[>C@8B7_PR4E>PI^<GCC)NNI;84$CU4-#
M5!.7Y+9528.P0==IP`L"GZK30N)-.G$^]L^"VK)X*MSH+PM)!*'1/?X%>*.;
M__W+_G`T&'_Y^#]1K5F/%^/_Q$GS?OY_B1^/3^Q%HW&_?\7#$^DYO/_?97,8
M3&41C$413@'(.0&A$W=1,P+\_'2P\>K%#U%_.KS4C:Z;#R^XF#?YB#LO)MEH
M__A@(>;.[&H&ZB2,[.S6(#U(V^KU30NUR"PSWW'P*UI_;N_!E.,DPW%O?C9;
MM_/]ZURE^)AON($Q&C-8CS)'`V5M?:N?7;+"=:[MO\OF>\='&IH"*R/3@Q.9
M:$!>"O-N.MR4XQE&=S-`3I[]>')\^/S@Q)3;P;>S63H;;*;OAE;O]FFS@5W6
MR_H5[<3A,H&S]^/K[\O1M^E0SZ7I@G:=38H&J5'>90V[:OEJ\WXSV_,'4S*N
M>(MA]7*\='T]VH[6Y3[N]>5EK2+AY>G5S,`X6\2$\NZ^6BEH<\$-S/A3CK/6
M!&9_<Z(00E0=OW[U8^?UR<OO_WS\M+/WS"])".>Q`F'_`Q!(/AE=R#UP^'L'
M-P(OX_#5X9ZMQ:"R7WJ,<XNP-5@4=<879WW<1HZ3DK:CVA6U$N"%AH<`TXQO
MX3JZV,`W>+[='&F.1X+28""?:0UOONF_];58S/&0HV:-]L?C]]%//+"U;7BM
MMNNZ*KLF^XF>[UH!Z+49@FTR[;L`DA'+0_EA.(I>&8I2(+=!.1^.3N1.USR0
M`['('0__>L>F](>SWHE<`IN'0BSZ'MT"A:2SW_*//*_7"&;Q,IV?W@D,8E^<3$SV
M/)CGAB;1HMF=P(Q6C36;<I`9A69^M7W'IO29?0$WAK$9H>A`W88;9%\-:K^S
M9P?[3G332_/ECQ$4YYBV+2G_]<<;`2"(SDD0$L-!.C1RS,.Y'9*1"+?!>5;Y
M37!.SM2N^>B1Y3Z_7L,WW3'*ZQFG,,>>S;:".W9>O/SSJZ????]Z)Y\S9((5
M,,`[\K]0F"QP/W_J\QCMNT6\N.S;4>?E\>O=;TRUKWYXNOO-UY7$(M,WN#>!
MZ3A(F9X/5TN<6T(:^'`&#XW`ENO.@3$K1]->)-?2(Q*#?DTO=6_5'U#G-W]`
M/?K%_/G\Q?/]9R\Z?W3GWTW^Q]8^;;M],<)AT_E8`R!<]JDQ1<5O9J5-]AO`
M0]S%BCO5_*//*_1H=RA:>>0\1S1W/P#N]/]I9A9NOX?V?ZO^'U=KB_$_DT;E7O__
M(K_5^K]1I^0:^.DPPYX8:".2.#-[WST]>&`O.+]1[U]6^__IM?[52G^@\P>Q
M@(25W+(&N($9+2T&5G(E[PCR3\^=%ETQ:N:?1X_<UJ(AG^*W$E#&_*//*_BBBHT'
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
>[G_WO_O?_>_^=_^[_]W_[G__JW[_'_@KJ&P`0`$`
`
end

*/
#endif
