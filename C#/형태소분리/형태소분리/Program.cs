using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace 형태소분리 {
    public struct HANGUL {
        public string isHangul;//한글인지 여부
        public char originalChar;//한글 원본
        public char[] chars;//분리한 자음 모음 
    }
    public class JaeumMoeum {
        public static readonly string ChosungArray = "ㄱㄲㄴㄷㄸㄹㅁㅂㅃㅅㅆㅇㅈㅉㅊㅋㅌㅍㅎ";//초성 19개
        public static readonly string JungsungArray = "ㅏㅐㅑㅒㅓㅔㅕㅖㅗㅘㅙㅚㅛㅜㅝㅞㅟㅠㅡㅢㅣ";//중성 21개
        public static readonly string JongsungArray = " ㄱㄲㄳㄴㄵㄶㄷㄹㄺㄻㄼㄽㄾㄿㅀㅁㅂㅄㅅㅆㅇㅈㅊㅋㅌㅍㅎ";//종성 28개
        private static readonly ushort m_UniCodeHangulBase = 0xAC00;
        private static readonly ushort m_UniCodeHangulLast = 0xD79F;

        public JaeumMoeum() { }
        public static char MergeJaeumMoeum(string choSung, string jungSung, string jongSung) {
            int ChoSungPosition, JungSungPosition, JongSungPosition;//초,중,종성의 위치를 저장할 변수
            int nUniCode;//초,중,종성을 합친 한글자의 유니코드값
            char retLetter;//초,중,종성을 합쳐 반환할 한 글자

            ChoSungPosition = ChosungArray.IndexOf(choSung);// 초성 위치
            JungSungPosition = JungsungArray.IndexOf(jungSung);// 중성 위치
            JongSungPosition = JongsungArray.IndexOf(jongSung);// 종성 위치

            //한 글자의 유니코드값 계산
            nUniCode = m_UniCodeHangulBase + (ChoSungPosition * 21 + JungSungPosition) * 28 + JongSungPosition;

            //유니코드 값을 문자로 변환
            retLetter = Convert.ToChar(nUniCode);
            //합쳐진 한글자 반환 
            return retLetter;
        }
        public static HANGUL DevideJaeumMoeum(char hangul) {
            int ChoSung, JungSung, JongSung;    // 초성,중성,종성의 인덱스
            ushort temp = 0x0000;                // 임시로 코드값을 담을 변수
            HANGUL hg = new HANGUL();

            //Char을 16비트 부호없는 정수형 형태로 변환 - Unicode
            temp = Convert.ToUInt16(hangul);

            // 인자가 한글이 아닌 경우
            if ((temp < m_UniCodeHangulBase) || (temp > m_UniCodeHangulLast)) {
                hg.isHangul = "NH";
                hg.originalChar = hangul;
                hg.chars = null;
            }
            else {
                int nUniCode = temp - m_UniCodeHangulBase;
                ChoSung = nUniCode / (21 * 28);
                nUniCode = nUniCode % (21 * 28);
                JungSung = nUniCode / 28;
                nUniCode = nUniCode % 28;
                JongSung = nUniCode;

                hg.isHangul = "H";
                hg.originalChar = hangul;
                hg.chars = new char[] { ChosungArray[ChoSung], JungsungArray[JungSung], JongsungArray[JongSung] };
            }

            return hg;
        }
       
        public static void Main(string[] args) {
            int i,j,num;
            char[] word;
            string tmpWord;
            HANGUL hg = new HANGUL();


            Console.WriteLine("뽑아낼 글자 <1.초성 2.중성 3.종성>");
            num=int.Parse(Console.ReadLine());
            Console.Write("단어 입력: ");
            tmpWord=Console.ReadLine();

            word=new char[tmpWord.Length];

            for (i = 0; i < tmpWord.Length; i++) 
                word[i]=tmpWord[i];                
            
            switch (num) {
                case 1:
                    Console.WriteLine("--초성 출력--");
                    for(i=0;i < tmpWord.Length;i++){
                    hg = DevideJaeumMoeum(word[i]);
                        
                    Console.Write(hg.chars[0]);
                        
                    }
                    Console.WriteLine("");
                    break;
                case 2:
                    Console.WriteLine("--중성 출력--");
                    for (i = 0; i < tmpWord.Length; i++) {
                        hg = DevideJaeumMoeum(word[i]);

                        Console.Write(hg.chars[1]);

                    }
                    break;
                case 3:
                    Console.WriteLine("--종성 출력--");
                    for (i = 0; i < tmpWord.Length; i++) {
                        hg = DevideJaeumMoeum(word[i]);

                        Console.Write(hg.chars[2]);

                    }
                    break;
            }
        
        }
    }
}

