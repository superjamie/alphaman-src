    CASE 200   'debug - test
      SCREEN , , 3: COLOR 11, 1: ccls 3: LOCATE 1, 1
      FOR ijk = 1 TO ngoody
        IF ABS(goody(ijk, 1)) = 9 THEN
          PRINT "Item name: "; gdy(ijk); " should be ";
          SELECT CASE goody(ijk, 3)
            CASE 1: PRINT "Skipper's Hat";
            CASE 2: PRINT "Serum.  Item is ";
              IF goody(ijk, 5) = 0 THEN PRINT "not ";
              PRINT "distilled.";
            CASE 3: PRINT "Map";
            CASE 4: PRINT "Blue Suede Shoes";
            CASE 5: PRINT "SpaceSuit";
            CASE 6: PRINT "Roast Beast";
            CASE 7: PRINT "Bamboo Raft.  Item is ";
              IF goody(ijk, 5) = 0 THEN PRINT "not ";
              PRINT "sped up.  It has"; goody(ijk, 4); " uses left.";
            CASE 8: PRINT "Mets Hat";
            CASE ELSE: PRINT "TYPE UNKNOWN!  Number:"; goody(ijk, 3);
          END SELECT
        END IF
        PRINT
      NEXT ijk
      didstuff = false: PauseForKey: SCREEN , , vpage
    CASE 201 ' debug, give S.hat,serum,map,shoes,suit,rbeast,braft,M.hat
      ijk = 0: dropped = 1
      WHILE ngoody < 20 AND ijk < 8
        ijk = ijk + 1: ngoody = ngoody + 1
        SELECT CASE ijk
          CASE 1: gdy(ngoody) = "SKIP'S HAT"
          CASE 2: gdy(ngoody) = "SERUM"
          CASE 3: gdy(ngoody) = "MAP"
          CASE 4: gdy(ngoody) = "SHOES": dropped = 2  'bsshoes
          CASE 5: gdy(ngoody) = "SUIT": dropped = 10  'suit
          CASE 6: gdy(ngoody) = "ROASTBEAST"
          CASE 7: gdy(ngoody) = "RAFT": dropped = 15  'bamboo raft
          CASE 8: gdy(ngoody) = "METS HAT"
        END SELECT
        DetermineSpecial dropped, ijk   'actually, dropped=mass
      WEND
      didstuff = false

