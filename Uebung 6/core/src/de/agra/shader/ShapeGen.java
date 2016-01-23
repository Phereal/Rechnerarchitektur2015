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
    
    public static float getSphereKarthX (float r, float theta, float phi){
        double x = r * Math.cos(Math.toRadians(theta)) * Math.cos(Math.toRadians(phi));
        return (float) x;
    }
    
    public static float getSphereKarthY (float r, float theta, float phi){
        double y = r * Math.cos(Math.toRadians(theta)) * Math.sin(Math.toRadians(phi));
        return (float) y;
    }
    
    public static float getSphereKarthZ (float r, float theta, float phi){
        double z = r * Math.sin(Math.toRadians(theta));
        return (float) z;
    }
}
