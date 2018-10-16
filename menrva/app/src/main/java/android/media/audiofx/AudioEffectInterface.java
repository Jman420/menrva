package android.media.audiofx;

import java.util.UUID;

public class AudioEffectInterface {
    public static AudioEffect CreateAudioEffect(UUID type, UUID id, int priority, int audioSessionId) {
        AudioEffect result = new AudioEffect(type, id, priority, audioSessionId);
        return result;
    }
}
