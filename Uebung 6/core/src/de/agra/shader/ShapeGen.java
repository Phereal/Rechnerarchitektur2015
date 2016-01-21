package de.agra.shader;
import java.lang.Math;

/**
 * Created by Jemand on 20.01.2016.
 */
public class ShapeGen {

    /**
     * Gibt die X-Koordinates eines Punktes in einem Kreis zurück
     *
     * @param radius des Kreises
     * @param degrees der X-Koordinate
     * @return
     */
    public static float getCirclePointX (float radius, float degrees){
        double x = (radius * Math.cos(Math.toRadians(degrees)));
        return (float) x;
    }

    public static float getCirclePointZ (float radius, float degrees){
        double Z = (radius * Math.sin(Math.toRadians(degrees)));
        return (float) Z;
    }
}
