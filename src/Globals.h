/*
 *  Copyright (C) 2011-2021 Team Kodi (https://kodi.tv)
 *  Copyright (C) 2011 Pulse-Eight (http://www.pulse-eight.com/)
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSE.md for more information.
 */

#include <string>

static const std::string EON_USER_AGENT = std::string("Kodi/")
    + std::string(STR(KODI_VERSION)) + std::string(" pvr.ard/")
    + std::string(STR(EON_VERSION));

static const std::string GLOBAL_URL = "global.united.cloud/";
/*
[r.XH.ARD]: "",
 [r.XH.DAS_ERSTE]: "Das Erste",
 [r.XH.BR]: "BR",
 [r.XH.HR]: "hr",
 [r.XH.MDR]: "MDR",
 [r.XH.NDR]: "NDR",
 [r.XH.RADIO_BREMEN]: "Radio Bremen",
 [r.XH.RBB]: "rbb",
 [r.XH.SR]: "SR",
 [r.XH.SWR]: "SWR",
 [r.XH.WDR]: "WDR",
 [r.XH.ONE]: "ONE",
 [r.XH.FUNK]: "funk",
 [r.XH.ARTE]: "ARTE",
 [r.XH.DREI_SAT]: "3sat",
 [r.XH.ALPHA]: "ARD alpha",
 [r.XH.TAGESSCHAU24]: "tagesschau24",
 [r.XH.PHOENIX]: "phoenix",
 [r.XH.KIKA]: "KiKA",
 [r.XH.ZDF]: "ZDF"
})


[r.XH.DAS_ERSTE]: ["Y3JpZDovL2Rhc2Vyc3RlLmRlL2xpdmUvY2xpcC9hYmNhMDdhMy0zNDc2LTQ4NTEtYjE2Mi1mZGU4ZjY0NmQ0YzQ"],
           [r.XH.TAGESSCHAU24]: ["Y3JpZDovL2Rhc2Vyc3RlLmRlL3RhZ2Vzc2NoYXUvbGl2ZXN0cmVhbQ"],
           [r.XH.RADIO_BREMEN]: ["Y3JpZDovL3JhZGlvYnJlbWVuLmRlL3JhZGlvX2JyZW1lbl9saXZlc3RyZWFt"],
           [r.XH.ALPHA]: ["Y3JpZDovL2JyLmRlL0xpdmVzdHJlYW0tQVJELUFscGhh"],
           [r.XH.ARTE]: ["Y3JpZDovL2FydGUuZGUvTGl2ZXN0cmVhbS1BUlRF"],
           [r.XH.BR]: ["Y3JpZDovL2JyLmRlL0xpdmVzdHJlYW0tQlItU8O8ZA", "Y3JpZDovL2JyLmRlL0xpdmVzdHJlYW0tQlItTm9yZA"],
           [r.XH.HR]: ["Y3JpZDovL2hyLmRlL0xpdmVzdHJlYW0tSFI"],
           [r.XH.MDR]: ["Y3JpZDovL21kci5kZS9MaXZlc3RyZWFtLU1EUi1TYWNoc2Vu", "Y3JpZDovL21kci5kZS9MaXZlc3RyZWFtLU1EUi1TYWNoc2VuLUFuaGFsdA", "Y3JpZDovL21kci5kZS9MaXZlc3RyZWFtLU1EUi1UaMO8cmluZ2Vu"],
           [r.XH.NDR]: ["Y3JpZDovL25kci5kZS9MaXZlc3RyZWFtLU5EUi1OaWVkZXJzYWNoc2Vu", "Y3JpZDovL25kci5kZS9MaXZlc3RyZWFtLU5EUi1IYW1idXJn", "Y3JpZDovL25kci5kZS9MaXZlc3RyZWFtLU5EUi1NZWNrbGVuYnVyZy1Wb3Jwb21tZXJu", "Y3JpZDovL25kci5kZS9MaXZlc3RyZWFtLU5EUi1OaWVkZXJzYWNoc2Vu", "Y3JpZDovL25kci5kZS9MaXZlc3RyZWFtLU5EUi1TY2hsZXN3aWctSG9sc3RlaW4"],
           [r.XH.RBB]: ["Y3JpZDovL3JiYi1vbmxpbmUuZGUvcmJiZmVybnNlaGVuL2xpdmVfYnJhbmRlbmJ1cmcvc2VuZGVwbGF0ei0tLWxpdmVzdHJlYW0tLS1icmFuZGVuYnVyZy0tLWhsczE", "Y3JpZDovL3JiYi1vbmxpbmUuZGUvcmJiZmVybnNlaGVuL2xpdmVfYmVybGluL3NlbmRlcGxhdHotLS1saXZlc3RyZWFtLS0tYmVybGluLS0taGxzMQ", "Y3JpZDovL3JiYi1vbmxpbmUuZGUvcmJiZmVybnNlaGVuL2xpdmVfYnJhbmRlbmJ1cmcvc2VuZGVwbGF0ei0tLWxpdmVzdHJlYW0tLS1icmFuZGVuYnVyZy0tLWhsczE"],
           [r.XH.SR]: ["Y3JpZDovL3NyLW9ubGluZS5kZS8yODQ4NjAvbGl2ZXN0cmVhbQ"],
           [r.XH.SWR]: ["Y3JpZDovL3N3ci5kZS8xMzQ4MTA0Mg", "Y3JpZDovL3N3ci5kZS8xMzQ4MTIzMA"],
           [r.XH.WDR]: ["Y3JpZDovL3dkci5kZS9CZWl0cmFnLTNkYTY2NGRlLTE4YzItNDY1MC1hNGZmLTRmNjQxNDcyMDcyYg"],
           [r.XH.ONE]: ["Y3JpZDovL3dkci5kZS9CZWl0cmFnLTFlNjA0YWFlLTViODctNGMzNC04ZDhmLTg4OWI1ZjE2ZDU3Mw"],
           [r.XH.PHOENIX]: ["Y3JpZDovL3dkci5kZS9CZWl0cmFnLTE3YTg4ZDdmLWI5NTAtNDcyNy05M2E0LWE3NzI3YjkxNjVkZQ"],
           [r.XH.DREI_SAT]: ["Y3JpZDovLzNzYXQuZGUvTGl2ZXN0cmVhbS0zc2F0"],
           [r.XH.KIKA]: ["Y3JpZDovL2tpa2EuZGUvTGl2ZXN0cmVhbS1LaUth"],
           dw: ["Y3JpZDovL2RldXRzY2hld2VsbGUuZGUvTGl2ZXN0cmVhbS1EZXV0c2NoZVdlbGxl"],
           zdf: ["Y3JpZDovL3pkZi5kZS9MaXZlc3RyZWFtLXpkZg"]
       });
       let v;
       !function(e) {
           e.ZDF_LIVESTREAM_PAGE = "https://www.zdf.de/live-tv?xtor=CS5-167",
           e.SOPHORA_HOST = "https://www.daserste.de/ard/die-ard",
           e.SEARCH_LINKING_HOST = "https://api.ard.de/search-linking",
           e.PROGRAM_API_HOST = "https://programm-api.ard.de/nownext/api",
           e.NEWSLETTER = "https://seu2.cleverreach.com/f/136025-139443",
           e.HELP = "https://hilfe.ardmediathek.de",
           e.ARD_PLUS = "https://www.ardplus.de",
           e.TAGESSCHAU = "https://www.tagesschau.de",
           e.SPORTSCHAU = "https://www.sportschau.de",
           e.AUDIOTHEK = "https://www.ardaudiothek.de",
           e.KIKA = "https://www.kika.de"
       }(v || (v = {}));
       Object.freeze({
           Y3JpZDovL2Rhc2Vyc3RlLmRlL0xpdmVzdHJlYW0tRGFzRXJzdGU: g[r.XH.DAS_ERSTE][0]
       })
   },
   e.ARD = "ard",
   e.DAS_ERSTE = "daserste",
   e.BR = "br",
   e.HR = "hr",
   e.MDR = "mdr",
   e.NDR = "ndr",
   e.RADIO_BREMEN = "radiobremen",
   e.RBB = "rbb",
   e.SR = "sr",
   e.SWR = "swr",
   e.WDR = "wdr",
   e.ONE = "one",
   e.ARTE = "arte",
   e.FUNK = "funk",
   e.KIKA = "kika",
   e.DREI_SAT = "3sat",
   e.ALPHA = "alpha",
   e.TAGESSCHAU24 = "tagesschau24",
   e.PHOENIX = "phoenix",
   e.ZDF = "zdf"
*/
