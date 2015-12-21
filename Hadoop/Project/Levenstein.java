public class Levenstein {

  public static int levensteinDistance(String s1, String s2) {
    int[][] distanceMatrix = new int[s1.length() + 1][s2.length() + 1];

    for (int i = 1; i < s1.length() + 1; i++) {
      distanceMatrix[i][0] = i;
    }

    for (int j = 1; j < s2.length() + 1; j++) {
      distanceMatrix[0][j] = j;
    }

    for (int j = 0; j < s2.length(); j++) {
      for (int i = 0; i < s1.length(); i++) {

        if (s1.charAt(i) == s2.charAt(j)) {
          distanceMatrix[i + 1][j + 1] = distanceMatrix[i][j];
        } else {
          distanceMatrix[i + 1][j + 1] = Math.min(Math.min(distanceMatrix[i][j + 1] + 1, distanceMatrix[i + 1][j] + 1),
              distanceMatrix[i][j] + 1);
        }
      }
    }

    /* Print distance matrix
     *  printDebug(distanceMatrix, s1.size(), s2.size());
     */

    return distanceMatrix[s1.length()][s2.length()];
  }

  private static void printDebug(int[][] distanceMatrix, int sizeRow, int sizeCol) {
    for (int i = 0; i < sizeRow + 1; i++) {
      for (int j = 0; j < sizeCol + 1; j++) {
        System.out.print(distanceMatrix[i][j] + " ");
      }
      System.out.println();
    }
  }

  /*
   * assert levensteinDistance("niche", "chiens") == 5 : "Wrong distance";
   * assert levensteinDistance("Sunday", "Saturday") == 3 : "Wrong distance";
   * assert levensteinDistance("sitting", "kitten") == 3 : "Wrong distance";
   */
}
